
#include <KNoTThing.h>
#include <DYIRDaikin.h>

KNoTThing thing;
DYIRDaikin irdaikin;

#define THING_NAME "Remote Control"

#define ON_OFF_ID 1
#define ON_OFF_NAME "on/off"

int32_t isOn;

void default_config()
{
  irdaikin.setSwing_off(); //Swing
  irdaikin.setMode(1);     //mode [1 -> cold]
  irdaikin.setFan(5);      //speed fan [1,2,3,4, AUTO -> 5]
  irdaikin.setTemp(19);    //temperature
}

void op_swtich(int32_t value)
{

  switch (value)
  {
  case 0:
    off();
    break;

  case 1:
    on();
    break;

  default:
    temperature(value);
    break;
  }
}

int off()
{
  irdaikin.off();
  irdaikin.sendCommand();
}

int on()
{
  irdaikin.on();
  default_config();
  irdaikin.sendCommand();
}

int temperature(int32_t value)
{
  irdaikin.setTemp((int)value);
  irdaikin.sendCommand();
}


static int on_off_read(int32_t *val, int32_t *multiplier)
{
  *val = isOn;
  return 0;
}

static int on_off_write(int32_t *val, int32_t *multiplier)
{
  isOn = *val;
  op_swtich(*val);
  return 0;
}

void setup(void)
{
  Serial.begin(9600);
  irdaikin.begin(3);

  /* init KNoTThing library */
  thing.init(THING_NAME);
  /* Register the LED as a data source/sink */
  thing.registerIntData(ON_OFF_NAME, ON_OFF_ID, KNOT_TYPE_ID_TEMPERATURE,
                        KNOT_UNIT_TEMPERATURE_C, on_off_read, on_off_write);

  /* Send data every 30 seconds */
  thing.registerDefaultConfig(ON_OFF_ID,
                              KNOT_EVT_FLAG_TIME | KNOT_EVT_FLAG_CHANGE,
                              30, 0, 0, 0, 0);

  /* Print thing name via Serial */
  Serial.println(F(THING_NAME));
}

void loop(void)
{
  thing.run();
}
