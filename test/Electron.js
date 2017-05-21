////////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------------- //
//                                                                            //
//                       (C) 2017-2017 Robot Developers                       //
//                       See LICENSE for licensing info                       //
//                                                                            //
// -------------------------------------------------------------------------- //
////////////////////////////////////////////////////////////////////////////////

"use strict";

// Simple Electron wrapper, with a no-op GUI, to run Test.js inside the Electron runtime
var fs = require ('fs');
var vm = require ('vm');
var m = require ('module');
var path = require ('path');
var electron = require ('electron');
var app = electron.app;
var BrowserWindow = electron.BrowserWindow;

var filename = 'Test.js';

var mainWindow = null;
app.on('ready', function() {
	try {
		console.log('Electron version: ' + process.versions.electron);
		mainWindow = new BrowserWindow({show: false});
		var src = m.wrap("\n" + fs.readFileSync(path.join(__dirname, filename)));
		var func = vm.runInThisContext(src, {filename: filename, lineOffset: -1});
		func(undefined, require, undefined, filename, __dirname, process, global, Buffer);
	} catch(e) {
		console.error(e.stack);
		process.exit(1);
	}
	mainWindow.close();
});

app.on('window-all-closed', function() {
	app.quit();
});
