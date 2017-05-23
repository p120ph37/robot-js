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

var matrix = [
//	{ abi: 57, node: '8.0.0'					  },
//	{ abi: 56,					electron: '1.9.0' },
//	{ abi: 55,					electron: '1.8.0' },
//	{ abi: 54,					electron: '1.7.0' },
	{ abi: 53,					electron: '1.6.9' },
	{ abi: 51, node: '7.7.4'					  },
	// electron 1.5.x was beta and never fully released
	{ abi: 50,					electron: '1.4.16'},
	{ abi: 49,					electron: '1.3.15'},
	// electron 1.2.x incorrectly claims to be node-abi-48, but isn't
	// see: https://github.com/electron/electron/issues/5851
	{ abi: 48, node: '6.10.3'					  },
	{ abi: 47, node: '5.12.0'					  },
	{ abi: 46, node: '4.8.3'					  },
	{ abi: 14, node: '0.12.17'					  },
];

for(var i = 0; i < matrix.length; i++) {
	// prefer to build using node headers (even if electron shares this ABI)
	var build_runtime = matrix[i].node ? 'node' : 'electron';
	log('Compiling against ' + build_runtime + '=' + matrix[i][build_runtime]);
	run(
	  './node_modules/.bin/node-pre-gyp', 'rebuild',
	  '--runtime=' + build_runtime,
	  '--target=' + matrix[i][build_runtime]
	);
	// run tests
	if(matrix[i].node) {
		log('Testing against node=' + matrix[i].node);
		run_with(matrix[i].node,'npm', 'run', 'test');
		log('Test completed against node=' + matrix[i].node);
	}
	if(matrix[i].electron) {
		log('Testing against electron=' + matrix[i].electron);
		run('npm', 'install', '-g', 'electron@' + matrix[i].electron);
		if(process.platform === 'linux') {
			run('xvfb-run', 'npm', 'run', 'electron_test');
		} else {
			run('npm', 'run', 'electron_test');
		}
		log('Test completed against electron=' + matrix[i].electron);
	}
	log('All tests completed!');
	log('Publishing...');
	run(
	  './node_modules/.bin/node-pre-gyp', 'package',
	  '--runtime=' + build_runtime,
	  '--target=' + matrix[i][build_runtime]
	);
	run(
	  './node_modules/.bin/node-pre-gyp-github', 'publish',
	  '--runtime=' + build_runtime,
	  '--target=' + matrix[i][build_runtime]
	);
}
