

var LED_scale = 670; // 900, 250, 670
var LEN = 256;
var MAX = Math.pow(LEN-1, 1/2.5);

var str = '';
for(var i=0; i<LEN; i++){
    var val = Math.round(Math.pow(i,1/2.5) * (LED_scale - 0) / (MAX - 0) );
    // console.log(val);
    str += '0x' + val.toString(16) + ', ';
}
console.log(str);
