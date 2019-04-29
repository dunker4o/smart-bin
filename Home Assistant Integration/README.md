# Smart Bin

This integration allows our existing Smart Bin project to integrate with an already setup Home Assistant hub (https://www.home-assistant.io/).
Note that it only provides values from the onboard ESP module and not from the movement detection board.

### Installation

Copy this folder to `<config_dir>/custom_components/smart_bin/`.

Add the following entry in your `configuration.yaml`:

```yaml
smartbin:
  - platform: smart_bin
    host: HOST_HERE
    username: USERNAME_HERE
    password: PASSWORD_HERE_OR_secrets.yaml
```
