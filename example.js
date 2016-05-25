const libui = require('.');

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
    },
  },
  {
    type: 'about',
    onClick: () => {
      console.log('about');
    },
  },
  {
    type: 'check',
    label: 'Check this',
    onClick: () => {
      console.log('check');
    },
  },
  {
    type: 'preferences',
    onClick: () => {
      console.log('preferences');
    },
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

/*
<Window>
  <View>
    <Group title="Controls">
      <Button onClick>Button</Button>
      <CheckBox onToggle>CheckBox</CheckBox>
      <TextInput onChange />
      <Text>Text</Text>
      <HorizontalSeparator />
      <DatePicker />
      <TimePicker />
      <DateTimePicker />
      <FontButton />
      <ColorButton />
    </Group>
  </View>

  <View>
    <Group title="Numbers">
      <SpinBox min=0 max=100 />
      <Slider min=0 max=100 />
      <ProgressBar value=50 />
    </Group>
    <Group title="Lists">
      <ComboBox>
        <ComboBox.Item>1</ComboBox.Item>
        <ComboBox.Item>2</ComboBox.Item>
        <ComboBox.Item>3</ComboBox.Item>
      </ComboBox>
      <EditableComboBox>
        <ComboBox.Item>1</ComboBox.Item>
        <ComboBox.Item>2</ComboBox.Item>
        <ComboBox.Item>3</ComboBox.Item>
      </EditableComboBox>
      <RadioButtons>
        <RadioButtons.Button>1</RadioButtons.Button>
        <RadioButtons.Button>2</RadioButtons.Button>
        <RadioButtons.Button>3</RadioButtons.Button>
      </RadioButtons>
      <Tab>
        <Tab.Page title="Page 1">
          <View>
          </View>
        </Tab.Page>
        <Tab.Page title="Page 2">
          <View>
          </View>
        </Tab.Page>
        <Tab.Page title="Page 3">
          <View>
          </View>
        </Tab.Page>
      </Tab>
    </Group>
  </View>
</Window>
*/
