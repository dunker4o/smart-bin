"""Platform for smart bin integration."""
import logging

import voluptuous as vol

import homeassistant.helpers.config_validation as cv
# Import the device class from the component that you want to support
from homeassistant.components.light import (
    ATTR_BRIGHTNESS, PLATFORM_SCHEMA, Light)
from homeassistant.const import CONF_HOST, CONF_PASSWORD, CONF_USERNAME

_LOGGER = logging.getLogger(__name__)

# Validation of the user's configuration
PLATFORM_SCHEMA = PLATFORM_SCHEMA.extend({
    vol.Required(CONF_HOST): cv.string,
    vol.Optional(CONF_USERNAME, default='admin'): cv.string,
    vol.Optional(CONF_PASSWORD): cv.string,
})


def setup_platform(hass, config, add_entities, discovery_info=None):
    """Set up the Smart Bin platform."""
    import smartbin

    # Assign configuration variables.
    # The configuration check takes care they are present.
    host     = config[CONF_HOST]
    username = config[CONF_USERNAME]
    password = config.get(CONF_PASSWORD)

    # Setup connection with devices/cloud
    hub = smartbin.Hub(host, username, password)

    # Verify that passed in configuration works
    if not hub.is_valid_login():
        _LOGGER.error("Could not connect to SmartBin hub")
        return

    # Add devices
    add_entities(SmartBin(smartBins) for smartBins in hub.bins())


class SmartBin(sBin):
    """Representation of a Smart Bin."""

    def __init__(self, smartbin):
        """Initialize a SmartBin."""
        self._bin      = smartbin
        self._name     = smartbin.name
        self._state    = None
        self._distance = None
        self._duration = None

    @property
    def name(self):
        """Return the display name of this bin."""
        return self._name

    @property
    def distance(self):
        """Return the distance between lid and trash in the bin.

        This method is optional. Removing it indicates to Home Assistant
        that distance is not supported for this bin.
        """
        return self._distance

    @property
    def duration(self):
        """Return the echo pulse duration between lid and trash in the bin.

        This method is optional. Removing it indicates to Home Assistant
        that duration is not supported for this bin.
        """
        return self._duration

    @property
    def is_full(self):
        """Return true if bin is full."""
        return self._state


    def update(self):
        """Fetch new data for this bin.

        This is the only method that should fetch new data for Home Assistant.
        """
        self._bin.update()
        self._state = self._light.is_full()
        self._distance = self._bin.distance
        self._duration = self._bin.duration
