const libui = require('./build/Release/libui');
new libui.Window({}, (err, msg) => {
  console.log(msg);
});
