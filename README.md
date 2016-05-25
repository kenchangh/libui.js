libui.js
========
Create desktop interfaces in JavaScript, backed by libui


Example
---

```javascript
const libui = require('./build/Release/libui');

libui.init();

const menu = new libui.Menu('File', [
  {
    label: 'Open',
    type: 'item',
    onClick: () => {
      const filepath = window.openFile();
      if (!filepath) {
        window.msgBox('No file selected', "Don't be alarmed");
        return;
      }
      window.msgBox('File selected', filepath);
    }
  },
  {
    label: 'Save',
    type: 'item',
    onClick: () => {
      const filepath = window.saveFile();
      if (!filepath) {
        window.msgBox('No file selected', "Don't be alarmed");
        return;
      }
      window.msgBox('File selected', filepath);
    }
  },
]); // has to come before window

const window = new libui.Window({
  title: 'Cool',
  width: 700,
  height: 700,
  hasMenu: true,
  margined: false,
  onClosing: (err) => {
    console.log('exited')
  },
});

libui.render();
```
