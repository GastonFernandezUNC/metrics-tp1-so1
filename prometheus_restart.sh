echo "Restarting Prometheus"
systemctl restart prometheus
echo -e "...\nPrometheus has been restarted\n\n"
echo "If you need to change the configuration, you can do so by editing the file /etc/prometheus/prometheus.yml and running this script again."
