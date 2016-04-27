"use strict";

var assert = require('assert');

function BufferReader(buffer) {
    buffer = buffer || new Buffer(0);
    assert(Buffer.isBuffer(buffer), 'A Buffer must be provided');
    this.buf = buffer;
    this.offset = 0;
}

BufferReader.prototype.append = function(buffer) {
    assert(Buffer.isBuffer(buffer), 'A Buffer must be provided');
    this.buf = Buffer.concat([this.buf, buffer]);
    return this;
};

BufferReader.prototype.tell = function() {
    return this.offset;
};

BufferReader.prototype.seek = function(pos) {
    assert(pos >= 0 && pos <= this.buf.length, 'Position is Invalid');
    this.offset = pos;
    return this;
};

BufferReader.prototype.move = function(diff) {
    assert(this.offset + diff >= 0 && this.offset + diff <= this.buf.length, 'Difference is Invalid');
    this.offset += diff;
    return this;
};


BufferReader.prototype.nextAll =
BufferReader.prototype.restAll = function() {
    var remain = this.buf.length - this.offset;
    assert(remain >= 0, 'Buffer is not in normal state: offset > totalLength');
    var buf = new Buffer(remain);
    this.buf.copy(buf, 0, this.offset);
    this.offset = this.buf.length;
    return buf;
};


BufferReader.prototype.nextBuffer = function(length) {
    assert(length >= 0, 'Length must be no negative');
    assert(this.offset + length <= this.buf.length, "Out of Original Buffer's Boundary");
    var buf = new Buffer(length);
    this.buf.copy(buf, 0, this.offset, this.offset + length);
    this.offset += length;
    return buf;
};

BufferReader.prototype.nextString = function(length, encoding) {
    assert(length >= 0, 'Length must be no negative');
    assert(this.offset + length <= this.buf.length, "Out of Original Buffer's Boundary");

    this.offset += length;
    return this.buf.toString(encoding, this.offset - length, this.offset);
};

BufferReader.prototype.nextStringZero = function(encoding) {
    // Find null by end of buffer
    for(var length = 0; length + this.offset < this.buf.length && this.buf[this.offset + length] !== 0x00; length++) ;
    
    assert(length <= this.buf.length && this.buf[this.offset + length] === 0x00, "Out of Original Buffer's Boundary");

    this.offset += length + 1;
    return this.buf.toString(encoding, this.offset - length - 1, this.offset - 1);
};


function MAKE_NEXT_READER(valueName, size) {
    valueName = cap(valueName);
    BufferReader.prototype['next' + valueName] = function() {
        assert(this.offset + size <= this.buf.length, "Out of Original Buffer's Boundary");
        var val = this.buf['read' + valueName](this.offset);
        this.offset += size;
        return val;
    };
}

function MAKE_NEXT_READER_BOTH(valueName, size) {
    MAKE_NEXT_READER(valueName + 'LE', size);
    MAKE_NEXT_READER(valueName + 'BE', size);
}

MAKE_NEXT_READER('Int8', 1);
MAKE_NEXT_READER('UInt8', 1);
MAKE_NEXT_READER_BOTH('UInt16', 2);
MAKE_NEXT_READER_BOTH('Int16', 2);
MAKE_NEXT_READER_BOTH('UInt32', 4);
MAKE_NEXT_READER_BOTH('Int32', 4);
MAKE_NEXT_READER_BOTH('Float', 4);
MAKE_NEXT_READER_BOTH('Double', 8);

function cap(str) {
    return str.charAt(0).toUpperCase() + str.slice(1);
}


module.exports = BufferReader;
