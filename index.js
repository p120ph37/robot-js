var path = require('path');
module.exports = require('@mapbox/node-pre-gyp').find(path.resolve(path.join(__dirname,'./package.json')))
