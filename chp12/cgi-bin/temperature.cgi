#!/bin/bash
echo "Content-type: text/html"
echo ""
echo '<html><head>'
echo '<meta charset="UTF-8">'
echo '<title>Pi Weather Sensor</title></head>'
echo '<body><h1>Pi Weather Sensor</h1><para>'
/usr/local/bin/dht
echo '</para></html>'

