-- a simple http server
srv=net.createServer(net.TCP) 
gpio.mode(1,gpio.INPUT)
gpio.mode(7,gpio.OUTPUT)
srv:listen(80,function(conn)
    conn:on("receive",function(conn,payload) print(payload) 
        gpio.write(7, gpio.HIGH)
        conn:send("{\n")
        raw_voltage = adc.read(0) - 233    -- 233 is 25C
        diff_degC   = raw_voltage / 6      -- 6 steps is 1C
        temperature  = diff_degC + 25      -- add/sub from 25
        conn:send("   \"temperature\" : ")
        conn:send(temperature)
        conn:send(",\n")
        conn:send("   \"button\" : ")
        if gpio.read(1)==1 then
            conn:send("true\n")
        else
            conn:send("false\n")
        end
        conn:send("}\n")
        gpio.write(7, gpio.LOW)
	conn:on("sent",function(conn) conn:close() end)
    end)
end)
