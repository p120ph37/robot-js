#!/usr/bin/env node
const { promisify } = require('util');
const process = require('process');
const path = require('path');
const exec = promisify(require('child_process').exec);
const readdir = promisify(require('fs').readdir);
const readFile = promisify(require('fs').readFile);

const { Octokit } = require("@octokit/rest");
const octokit = new Octokit({
  auth: process.env.GITHUB_TOKEN,
});

(async () => {
  const pkg = require('./package.json');
  const repoPath = (await exec('git remote get-url origin')).stdout.trim().replace(/^.*\/([^\/]+\/[^\/]+)$/, '$1').replace(/\.git$/, '');
  const dir = path.join(__dirname, `build/stage/${pkg.version}`);
  const [owner, repo] = repoPath.split('/');
  const tag = pkg.version;
  let release = {};
  try {
    release = await octokit.repos.getReleaseByTag({ owner, repo, tag });
    console.log('Reusing existing release...');
  } catch(e) {
    if(e.status = 404) {
      release = await octokit.repos.createRelease({ owner, repo, tag_name: tag,
        name: pkg.version,
        target_commitish: (await exec('git rev-parse HEAD')).stdout.trim(),
        body: `${pkg.name} v${pkg.version}`,
        draft: true,
        prerelease: pkg.version.indexOf('-') > -1 ? true : false,
      });
      console.log('Created new draft release release...');
    } else {
      throw e;
    }
  }
  const files = (await readdir(dir)).map(f => path.join(dir, f));
  console.log(`Uploading ${files.length} assets... (may take a while)`);
  const promises = files.map(async (file, i) => {
    const name = path.basename(file);
    const existing = release.data.assets.find(a => a.name === name);
    if(existing) {
      console.log(`[${i + 1}/${files.length}] Removing ${name} (id:${existing.id})`);
      await octokit.repos.deleteReleaseAsset({ owner, repo, asset_id: existing.id });
    }
    console.log(`[${i + 1}/${files.length}] Reading ${file}`);
    const data = await readFile(file);
    console.log(`[${i + 1}/${files.length}] Uploading ${name} (${data.length}B)`);
    let upload = await octokit.repos.uploadReleaseAsset({ owner, repo,
      release_id: release.data.id,
      name, data,
    });
  });
  await Promise.all(promises);
  console.log("Done!");
})().catch(e => {
  console.trace(e);
  process.exit(1);
});
