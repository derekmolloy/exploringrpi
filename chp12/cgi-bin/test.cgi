#!/bin/bash
echo "Content-type: text/html"
echo ""
echo '<html><head>'
echo '<meta charset="UTF-8">'
echo '<title>Hello Raspberry Pi</title></head>'
echo '<body><h1>Hello Raspberry Pi</h1><para>'
hostname
echo ' has been up '
uptime
echo '</para></html>'

