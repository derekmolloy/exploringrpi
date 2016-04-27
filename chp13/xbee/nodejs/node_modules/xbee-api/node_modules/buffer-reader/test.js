var assert = require('chai').assert,
    BufferBuilder = require('buffer-builder'),
    BufferReader = require('./index');


describe('BufferReader', function() {
    var sz = 'Old school is cool!';
    var buf;
    before(function() {
        var builder = new BufferBuilder();
        builder.appendString('hello');
        builder.appendUInt8(8);
        builder.appendInt32BE(1);
        builder.appendStringZero(sz);
        builder.appendDoubleLE(1.25e5);
        builder.appendFloatBE(1.2345);
        builder.appendFill(3, 4);
        buf = builder.get();
    });

    it('read', function() {
        var reader = new BufferReader(buf);
        assert.equal(reader.nextString(5), 'hello');
        assert.equal(reader.nextUInt8(), 8);
        assert.equal(reader.nextInt32BE(), 1);
        assert.equal(reader.nextStringZero(), sz);
        assert.equal(reader.nextDoubleLE(), 1.25e5);
        assert.closeTo(reader.nextFloatBE(), 1.2345, 0.00001);
        var rest = reader.restAll();
        assert.equal(rest.length, 4);
    });
});
