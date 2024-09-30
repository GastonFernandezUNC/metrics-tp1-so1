echo "Apagar Server ó Iniciarlo? 0/1"
read option

if [ $option -eq 0 ]; then
    sudo systemctl stop grafana-server
    sudo systemctl status grafana-server
    exit 0
elif [ $option -eq 1 ]; then
    sudo systemctl daemon-reload
    sudo systemctl start grafana-server
    sudo systemctl status grafana-server
    exit 0
fi

