////////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------------- //
//                                                                            //
//                       (C) 2010-2017 Robot Developers                       //
//                       See LICENSE for licensing info                       //
//                                                                            //
// -------------------------------------------------------------------------- //
////////////////////////////////////////////////////////////////////////////////

"use strict";

var child_process = require('child_process');
var fs = require('fs');
var path = require('path');

function log(s) {
	console.log(path.basename(__filename) + ': ' + s);
}

function die(s) {
	console.error(path.basename(__filename) + ': ' + s);
	process.exit(1);
}

function run() {
	log('run: ' + Array.prototype.join.call(arguments, ' '));
	var code = child_process.spawnSync(arguments[0], Array.prototype.slice.call(arguments, 1), {shell: true, stdio: 'inherit'}).status;
	if(code) process.exit(code);
}

// Run command in a subshell that uses a specific node version
function run_with(v) {
	var args = Array.prototype.slice.call(arguments, 1);
	var cv = child_process.execSync('node -v').toString().toString().trim().substr(1);
	if(cv === v) {
		// no need to do anything special - just launch the command.
		run.apply(null, args);
	} else {
		if(process.platform === 'win32') {
			// on AppVeyor Windows, use the pre-installed node versions.wrap this in nodevars.bat
			var nodevars = 'C:\\avvm\\node\\' + v + '\\' + (process.arch === 'ia32' ? 'x86' : 'x64') + '\\nodejs\\nodevars.bat';
			fs.existsSync(nodevars) || die('Node version ' + v + ' is not available in this VM');
			run.apply(null, [nodevars, '&'].concat(args));
		} else {
			// on Travis Linux and OSX, run in a subshell with an altered path
			run.apply(null, ['. ~/.nvm/nvm.sh; nvm use ' + v + ';'].concat(args));
		}
	}
}

var node_ver = process.argv[2] ? process.argv[2].split(' ') : [];
for(var v of node_ver) {
	log('Compiling, testing, packaging, and publishing for node v' + v);
	run(
	  path.join('node_modules', '.bin', 'node-pre-gyp'), 'configure', 'build',
	  '--runtime=node', '--target=' + v
	);
	// run tests
	run_with(v, 'npm', 'run', 'test:ci:node');
	// package
	run(
	  path.join('node_modules', '.bin', 'node-pre-gyp'), 'package',
	  '--runtime=node', '--target=' + v
	);
}

var electron_ver = process.argv[3] ? process.argv[3].split(' ') : [];  // don't bother with the older "electron-prebuilt" versions
for(var v of electron_ver) {
	log('Compiling, testing, packaging, and publishing for electron v' + v);
	run(
	  path.join('node_modules', '.bin', 'node-pre-gyp'), 'configure', 'build',
	  '--runtime=electron', '--target=' + v,
	  '--dist-url=https://atom.io/download/electron'
	);
	// run tests
	run('npm', 'install', '-g', 'electron@' + v);
	if(process.platform === 'linux') {
		run('xvfb-run', 'npm', 'run', 'test:ci:electron');
	} else {
		run('npm', 'run', 'test:ci:electron');
	}
	// package
	run(
	  path.join('node_modules', '.bin', 'node-pre-gyp'), 'package',
	  '--runtime=electron', '--target=' + v
	);
}
