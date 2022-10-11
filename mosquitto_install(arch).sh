
sudo pacman -S mosquitto mosquitto-clients

sudo systemctl enable mosquitto

sudo systemctl start mosquitto

mosquitto_sub -h localhost -t "#" -v

mosquitto_pub -h localhost -t "domotuto.com/prueba funcionamiento mqtt" -m "funciona"