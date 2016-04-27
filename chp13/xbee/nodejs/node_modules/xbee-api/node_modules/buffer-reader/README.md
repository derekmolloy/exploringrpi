# Node Buffer Reader

Make reading data from a binary buffer easily.


<!--- travis ci -->
[![Build Status](https://travis-ci.org/villadora/node-buffer-reader.png?branch=master)](https://travis-ci.org/villadora/node-buffer-reader)


## Install

npm install buffer-reader

## Usage

``` js
var BufferReader = require('buffer-reader');
var reader = new BufferReader(buffer);

// Read next N bytes as string
var str = reader.nextString(10);

// Read next N bytes as buffer
var buf = reader.nextBuffer(10);

// Read next 2 bytes as Int16LE
var i = reader.nextInt16LE();


// Read next 8 bytes as DoubleLE
var d = reader.nextDoubleLE();

// Get all rest bytes as buffer
var rest = reader.restAll();

// OutOfBound will raise exception
try {
    reader.nextString(1);
}cache(e) {
    //  handle exception
}

// Append buffer to reader
reader.append(newbuffer);

// Read again
reader.nextString(1);

```


## APIs

### new BufferReader([buffer])
Create a new reader, if no buffer provided, a empty buffer will be used.

### append(buffer)
Append new buffer to the end of current reader

### tell()
Return current position of the reader


### seek(pos)
Set new position of the reader, if the pos is invalid, an exception will be raised

### move(offset) 
Move the position of reader by offset, offset can be negative; it can be used to skip some bytes

### restAll()
Get all the remaining bytes as a Buffer

### nextBuffer(length)
Read a buffer with specified length

### nextString(length, [encoding])
Read next length of bytes as String, encoding default is 'utf8'

### next(Int8|UInt8)\[(Int16|UInt16|Int32|UInt32|Float|Double)(LE|BE)\]()
Read next bytes as given types, the value is just as the same format Buffer in nodejs doc.

## License

(The BSD License)

Copyright (c) 2013, Villa.Gao <jky239@gmail.com>;
All rights reserved.
