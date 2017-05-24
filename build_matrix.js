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

var node_ver = ['7.7.3', '6.10.0', '5.12.0', '4.8.0', '0.12.17'];
for(var v of node_ver) {
	log('Compiling, testing, packaging, and publishing for node v' + v);
	run(
	  path.join('node_modules', '.bin', 'node-pre-gyp'), 'configure', 'build',
	  '--runtime=node', '--target=' + v
	);
	// run tests
	run_with(v, 'npm', 'run', 'test');
	// package
	run(
	  path.join('node_modules', '.bin', 'node-pre-gyp'), 'package',
	  '--runtime=node', '--target=' + v
	);
}

var electron_ver = ['1.7.1', '1.6.8', '1.5.1', '1.4.16', '1.3.15'];  // don't bother with the older "electron-prebuilt" versions
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
		run('xvfb-run', 'npm', 'run', 'electron_test');
	} else {
		run('npm', 'run', 'electron_test');
	}
	// package
	run(
	  path.join('node_modules', '.bin', 'node-pre-gyp'), 'package',
	  '--runtime=electron', '--target=' + v
	);
}
