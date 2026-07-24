import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate, uart
from esphome.const import CONF_ID

DEPENDENCIES = ["uart"]
AUTO_LOAD = ["climate"]

tclac_ns = cg.esphome_ns.namespace("tclac")
tclacClimate = tclac_ns.class_("tclacClimate", climate.Climate, uart.UARTDevice, cg.PollingComponent)

if hasattr(climate, "climate_schema"):
    BASE_SCHEMA = climate.climate_schema(tclacClimate)
else:
    BASE_SCHEMA = climate.CLIMATE_SCHEMA.extend({
        cv.GenerateID(): cv.declare_id(tclacClimate),
    })

CONFIG_SCHEMA = BASE_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(tclacClimate),
    }
).extend(cv.polling_component_schema("5s")).extend(uart.UART_DEVICE_SCHEMA)

async def to_code(config):
    # استخدام new_Pvariable المصححة بدلاً من new_variable
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await climate.register_climate(var, config)
    await uart.register_uart_device(var, config)
    
