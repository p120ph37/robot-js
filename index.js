var path = require('path');
var fs = require('fs');

// Normally we would use node-pre-gyp to locate the correct binary, but in the interest of simplifying runtime
// dependencies for bundled applications, we can instead make assumptions about the specific possible binary
// names and locations, and compute the correct require here without using node-pre-gyp...
// 
// module.exports = require(require('@mapbox/node-pre-gyp').find(path.resolve(path.join(__dirname,'./package.json'))));
//

var package_json = JSON.parse(fs.readFileSync(path.resolve(path.join(__dirname,'./package.json'))));
module.exports = require(
  path.resolve(path.join(__dirname, package_json.binary.module_path, package_json.binary.module_name + '.node'))
    .replace('{platform}', process.platform)
    .replace('{arch}', process.arch)
    .replace('{napi_build_version}', package_json.binary.napi_versions[0])
);
