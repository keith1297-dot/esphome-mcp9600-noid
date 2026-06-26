import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import (
    CONF_ID,
    CONF_UPDATE_INTERVAL,
    UNIT_CELSIUS,
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
)

DEPENDENCIES = ["i2c"]

CONF_HOT_JUNCTION = "hot_junction"
CONF_COLD_JUNCTION = "cold_junction"
CONF_THERMOCOUPLE_TYPE = "thermocouple_type"

mcp9600_noid_ns = cg.esphome_ns.namespace("mcp9600_noid")
MCP9600NoIdComponent = mcp9600_noid_ns.class_(
    "MCP9600NoIdComponent", cg.PollingComponent, i2c.I2CDevice
)

ThermocoupleType = mcp9600_noid_ns.enum("ThermocoupleType")
THERMOCOUPLE_TYPES = {
    "K": ThermocoupleType.THERMOCOUPLE_TYPE_K,
    "J": ThermocoupleType.THERMOCOUPLE_TYPE_J,
    "T": ThermocoupleType.THERMOCOUPLE_TYPE_T,
    "N": ThermocoupleType.THERMOCOUPLE_TYPE_N,
    "S": ThermocoupleType.THERMOCOUPLE_TYPE_S,
    "E": ThermocoupleType.THERMOCOUPLE_TYPE_E,
    "B": ThermocoupleType.THERMOCOUPLE_TYPE_B,
    "R": ThermocoupleType.THERMOCOUPLE_TYPE_R,
}

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(MCP9600NoIdComponent),
            cv.Optional(CONF_THERMOCOUPLE_TYPE, default="K"): cv.enum(
                THERMOCOUPLE_TYPES, upper=True
            ),
            cv.Optional(CONF_HOT_JUNCTION): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_COLD_JUNCTION): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
        }
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(i2c.i2c_device_schema(0x60))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    cg.add(var.set_thermocouple_type(config[CONF_THERMOCOUPLE_TYPE]))

    if CONF_HOT_JUNCTION in config:
        sens = await sensor.new_sensor(config[CONF_HOT_JUNCTION])
        cg.add(var.set_hot_junction_sensor(sens))

    if CONF_COLD_JUNCTION in config:
        sens = await sensor.new_sensor(config[CONF_COLD_JUNCTION])
        cg.add(var.set_cold_junction_sensor(sens))
