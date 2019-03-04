/*
 * TMK keymap editor
 */

var a_table = "00000000 77073096 EE0E612C 990951BA 076DC419 706AF48F E963A535 9E6495A3 0EDB8832 79DCB8A4 E0D5E91E 97D2D988 09B64C2B 7EB17CBD E7B82D07 90BF1D91 1DB71064 6AB020F2 F3B97148 84BE41DE 1ADAD47D 6DDDE4EB F4D4B551 83D385C7 136C9856 646BA8C0 FD62F97A 8A65C9EC 14015C4F 63066CD9 FA0F3D63 8D080DF5 3B6E20C8 4C69105E D56041E4 A2677172 3C03E4D1 4B04D447 D20D85FD A50AB56B 35B5A8FA 42B2986C DBBBC9D6 ACBCF940 32D86CE3 45DF5C75 DCD60DCF ABD13D59 26D930AC 51DE003A C8D75180 BFD06116 21B4F4B5 56B3C423 CFBA9599 B8BDA50F 2802B89E 5F058808 C60CD9B2 B10BE924 2F6F7C87 58684C11 C1611DAB B6662D3D 76DC4190 01DB7106 98D220BC EFD5102A 71B18589 06B6B51F 9FBFE4A5 E8B8D433 7807C9A2 0F00F934 9609A88E E10E9818 7F6A0DBB 086D3D2D 91646C97 E6635C01 6B6B51F4 1C6C6162 856530D8 F262004E 6C0695ED 1B01A57B 8208F4C1 F50FC457 65B0D9C6 12B7E950 8BBEB8EA FCB9887C 62DD1DDF 15DA2D49 8CD37CF3 FBD44C65 4DB26158 3AB551CE A3BC0074 D4BB30E2 4ADFA541 3DD895D7 A4D1C46D D3D6F4FB 4369E96A 346ED9FC AD678846 DA60B8D0 44042D73 33031DE5 AA0A4C5F DD0D7CC9 5005713C 270241AA BE0B1010 C90C2086 5768B525 206F85B3 B966D409 CE61E49F 5EDEF90E 29D9C998 B0D09822 C7D7A8B4 59B33D17 2EB40D81 B7BD5C3B C0BA6CAD EDB88320 9ABFB3B6 03B6E20C 74B1D29A EAD54739 9DD277AF 04DB2615 73DC1683 E3630B12 94643B84 0D6D6A3E 7A6A5AA8 E40ECF0B 9309FF9D 0A00AE27 7D079EB1 F00F9344 8708A3D2 1E01F268 6906C2FE F762575D 806567CB 196C3671 6E6B06E7 FED41B76 89D32BE0 10DA7A5A 67DD4ACC F9B9DF6F 8EBEEFF9 17B7BE43 60B08ED5 D6D6A3E8 A1D1937E 38D8C2C4 4FDFF252 D1BB67F1 A6BC5767 3FB506DD 48B2364B D80D2BDA AF0A1B4C 36034AF6 41047A60 DF60EFC3 A867DF55 316E8EEF 4669BE79 CB61B38C BC66831A 256FD2A0 5268E236 CC0C7795 BB0B4703 220216B9 5505262F C5BA3BBE B2BD0B28 2BB45A92 5CB36A04 C2D7FFA7 B5D0CF31 2CD99E8B 5BDEAE1D 9B64C2B0 EC63F226 756AA39C 026D930A 9C0906A9 EB0E363F 72076785 05005713 95BF4A82 E2B87A14 7BB12BAE 0CB61B38 92D28E9B E5D5BE0D 7CDCEFB7 0BDBDF21 86D3D2D4 F1D4E242 68DDB3F8 1FDA836E 81BE16CD F6B9265B 6FB077E1 18B74777 88085AE6 FF0F6A70 66063BCA 11010B5C 8F659EFF F862AE69 616BFFD3 166CCF45 A00AE278 D70DD2EE 4E048354 3903B3C2 A7672661 D06016F7 4969474D 3E6E77DB AED16A4A D9D65ADC 40DF0B66 37D83BF0 A9BCAE53 DEBB9EC5 47B2CF7F 30B5FFE9 BDBDF21C CABAC28A 53B39330 24B4A3A6 BAD03605 CDD70693 54DE5729 23D967BF B3667A2E C4614AB8 5D681B02 2A6F2B94 B40BBE37 C30C8EA1 5A05DF1B 2D02EF8D";
var b_table = a_table.split(' ').map(function(s){ return parseInt(s,16) });
function b_crc32 (data) {
    var crc = -1;
    for(var i=0; i<data.length; i++) {
        crc = ( crc >>> 8 ) ^ b_table[( crc ^ data[i] ) & 0xFF];
    }
    return (crc ^ (-1)) >>> 0;
};

function Uint32ToByteArray (u32) {
    // we want to represent the input as a 4-bytes array
    var byteArray = [0, 0, 0, 0];

    for ( var index = 0; index < byteArray.length; index ++ ) {
        var b = u32 & 0xff;
        byteArray [ index ] = b;
        u32 = u32 >> 8 ;
    }

    return byteArray;
};


/*
 * Share URL
 */
function encode_keymap(keymap)
{
    return window.btoa(JSON.stringify(keymap));
}

function decode_keymap(hash)
{
    try {
        return JSON.parse(window.atob(hash));
    } catch (err) {
        return null;
    }
}

/*
 * Hex file
 */
function hexstr2(b)
{
    return ('0'+ b.toString(16)).substr(-2).toUpperCase();
}

function hex_line(record_type, data)
{
    var sum = 0;
    sum += data.length;
    sum += record_type;

    var line = '';
    line += ':';
    line += hexstr2(data.length);
    line += hexstr2(record_type);
    for (var i = 0; i < data.length; i++) {
        sum = (sum + data[i]);
        line += hexstr2(data[i]);
    }
    line += hexstr2((~sum + 1)&0xff);  // Checksum
    line +="\r\n";
    return line;
}

function hex_eof()
{
    return ":00000001FF\r\n";
}

function hex_output(data) {
    var line = [];

    // TODO: refine: flatten data into one dimension array
    [].concat.apply([], [].concat.apply([], data)).forEach(function(e) {
        line.push(e);
    });
    return line;
}


function check_fnkey(maparray) {
    var hasItFn = 0;
    for (var row in maparray[0]) {
        for (var col in maparray[0][row]) {
            var k = maparray[0][row][col]; 
            if (k == 255) {
                hasItFn = 1;
            }
        }
    }
    if (hasItFn == 0) {
        alert("Keymap layer#0 MUST have Fn key");
    }
    return hasItFn;
}



/* 
 * keycodes
 */
var code_display = [
    // {id, name(text), description(tooltip)}
    {id: 'NO  ',                        name: 'NO',                          desc: 'No action'},
    {id: 'TRNS',                        name: 'TRNS',                        desc: 'Transparent'},
    {id: 'POST_FAIL',                   name: 'POST_FAIL',                   desc: 'POST_FAIL'},
    {id: 'UNDEFINED',                   name: 'UNDEFINED',                   desc: 'UNDEFINED'},
    {id: 'A',                           name: 'A',                           desc: 'A'},
    {id: 'B',                           name: 'B',                           desc: 'B'},
    {id: 'C',                           name: 'C',                           desc: 'C'},
    {id: 'D',                           name: 'D',                           desc: 'D'},
    {id: 'E',                           name: 'E',                           desc: 'E'},
    {id: 'F',                           name: 'F',                           desc: 'F'},
    {id: 'G',                           name: 'G',                           desc: 'G'},
    {id: 'H',                           name: 'H',                           desc: 'H'},
    {id: 'I',                           name: 'I',                           desc: 'I'},
    {id: 'J',                           name: 'J',                           desc: 'J'},
    {id: 'K',                           name: 'K',                           desc: 'K'},
    {id: 'L',                           name: 'L',                           desc: 'L'},
    {id: 'M',                           name: 'M',                           desc: 'M'},
    {id: 'N',                           name: 'N',                           desc: 'N'},
    {id: 'O',                           name: 'O',                           desc: 'O'},
    {id: 'P',                           name: 'P',                           desc: 'P'},
    {id: 'Q',                           name: 'Q',                           desc: 'Q'},
    {id: 'R',                           name: 'R',                           desc: 'R'},
    {id: 'S',                           name: 'S',                           desc: 'S'},
    {id: 'T',                           name: 'T',                           desc: 'T'},
    {id: 'U',                           name: 'U',                           desc: 'U'},
    {id: 'V',                           name: 'V',                           desc: 'V'},
    {id: 'W',                           name: 'W',                           desc: 'W'},
    {id: 'X',                           name: 'X',                           desc: 'X'},
    {id: 'Y',                           name: 'Y',                           desc: 'Y'},
    {id: 'Z',                           name: 'Z',                           desc: 'Z'},
    {id: '1',                           name: '1',                           desc: '1'},
    {id: '2',                           name: '2',                           desc: '2'},
    {id: '3',                           name: '3',                           desc: '3'},
    {id: '4',                           name: '4',                           desc: '4'},
    {id: '5',                           name: '5',                           desc: '5'},
    {id: '6',                           name: '6',                           desc: '6'},
    {id: '7',                           name: '7',                           desc: '7'},
    {id: '8',                           name: '8',                           desc: '8'},
    {id: '9',                           name: '9',                           desc: '9'},
    {id: '0',                           name: '0',                           desc: '0'},
    {id: 'ENT',                         name: 'Enter',                       desc: 'ENTER'},
    {id: 'ESC',                         name: 'Esc',                         desc: 'Escape'},
    {id: 'BSPC',                        name: 'Back space',                  desc: 'Backspace'},
    {id: 'TAB',                         name: 'Tab',                         desc: 'Tab'},
    {id: 'SPC',                         name: 'Space',                       desc: 'Space'},
    {id: 'MINS',                        name: '-',                           desc: 'MINUS'},
    {id: 'EQL',                         name: '=',                           desc: 'EQUAL'},
    {id: 'LBRC',                        name: '[',                           desc: 'Left Bracket'},
    {id: 'RBRC',                        name: ']',                           desc: 'Right Bracket'},
    {id: 'BSLS',                        name: "\\",                          desc: 'Backslash'},
    {id: 'NUHS',                        name: 'ISO #',                       desc: 'Non-US Hash'},
    {id: 'SCLN',                        name: ';',                           desc: 'Semicolon'},
    {id: 'QUOT',                        name: "'",                           desc: 'Quote'},
    {id: 'GRV ',                        name: '`',                           desc: 'Grave'},
    {id: 'COMM',                        name: ',',                           desc: 'Comma'},
    {id: 'DOT ',                        name: '.',                           desc: 'Dot'},
    {id: 'SLSH',                        name: '/',                           desc: 'Slash'},
    {id: 'CAPS',                        name: 'Caps Lock',                   desc: 'Need this? Sure? :)'},
    {id: 'F1  ',                        name: 'F1',                          desc: 'F1'},
    {id: 'F2  ',                        name: 'F2',                          desc: 'F2'},
    {id: 'F3  ',                        name: 'F3',                          desc: 'F3'},
    {id: 'F4  ',                        name: 'F4',                          desc: 'F4'},
    {id: 'F5  ',                        name: 'F5',                          desc: 'F5'},
    {id: 'F6  ',                        name: 'F6',                          desc: 'F6'},
    {id: 'F7  ',                        name: 'F7',                          desc: 'F7'},
    {id: 'F8  ',                        name: 'F8',                          desc: 'F8'},
    {id: 'F9  ',                        name: 'F9',                          desc: 'F9'},
    {id: 'F10 ',                        name: 'F10',                         desc: 'F10'},
    {id: 'F11 ',                        name: 'F11',                         desc: 'F11'},
    {id: 'F12 ',                        name: 'F12',                         desc: 'F12'},
    {id: 'PSCR',                        name: 'Print Screen',                desc: 'Print Screen'},
    {id: 'SLCK',                        name: 'Scroll Lock',                 desc: 'Scroll Lock'},
    {id: 'PAUS',                        name: 'Pause',                       desc: 'Pause'},
    {id: 'INS ',                        name: 'Insert',                      desc: 'Insert'},
    {id: 'HOME',                        name: 'Home',                        desc: 'Home'},
    {id: 'PGUP',                        name: 'Page Up',                     desc: 'Page Up'},
    {id: 'DEL ',                        name: 'Delete',                      desc: 'Delete'},
    {id: 'END ',                        name: 'End',                         desc: 'End'},
    {id: 'PGDN',                        name: 'Page Down',                   desc: 'Page Down'},
    {id: 'RGHT',                        name: '\u2192',                      desc: 'Right'},
    {id: 'LEFT',                        name: '\u2190',                      desc: 'Left'},
    {id: 'DOWN',                        name: '\u2193',                      desc: 'Down'},
    {id: 'UP  ',                        name: '\u2191',                      desc: 'Up'},
    {id: 'NLCK',                        name: 'Num Lock',                    desc: 'Num Lock'},
    {id: 'PSLS',                        name: 'P/',                          desc: 'Keypad Slash'},
    {id: 'PAST',                        name: 'P*',                          desc: 'Keypad Asterisk'},
    {id: 'PMNS',                        name: 'P-',                          desc: 'Keypad Minus'},
    {id: 'PPLS',                        name: 'P+',                          desc: 'Keypad Plus'},
    {id: 'PENT',                        name: 'P\u21A9',                     desc: 'Keypad Enter'},
    {id: 'P1  ',                        name: 'P1',                          desc: 'Keypad 1'},
    {id: 'P2  ',                        name: 'P2',                          desc: 'Keypad 2'},
    {id: 'P3  ',                        name: 'P3',                          desc: 'Keypad 3'},
    {id: 'P4  ',                        name: 'P4',                          desc: 'Keypad 4'},
    {id: 'P5  ',                        name: 'P5',                          desc: 'Keypad 5'},
    {id: 'P6  ',                        name: 'P6',                          desc: 'Keypad 6'},
    {id: 'P7  ',                        name: 'P7',                          desc: 'Keypad 7'},
    {id: 'P8  ',                        name: 'P8',                          desc: 'Keypad 8'},
    {id: 'P9  ',                        name: 'P9',                          desc: 'Keypad 9'},
    {id: 'P0  ',                        name: 'P0',                          desc: 'Keypad 0'},
    {id: 'PDOT',                        name: 'P.',                          desc: 'Keypad Dot'},
    {id: 'NUBS',                        name: 'ISO \\',                      desc: 'Non-US Backslash'},
    {id: 'APP ',                        name: 'Application',                 desc: 'Application'},
    {id: 'POWER',                       name: '_Power',                      desc: 'Power(Not work on Windows)'},
    {id: 'PEQL',                        name: 'P=',                          desc: 'Keymapd Equal'},
    {id: 'F13 ',                        name: 'F13',                         desc: 'F13'},
    {id: 'F14 ',                        name: 'F14',                         desc: 'F14'},
    {id: 'F15 ',                        name: 'F15',                         desc: 'F15'},
    {id: 'F16 ',                        name: 'F16',                         desc: 'F16'},
    {id: 'F17 ',                        name: 'F17',                         desc: 'F17'},
    {id: 'F18 ',                        name: 'F18',                         desc: 'F18'},
    {id: 'F19 ',                        name: 'F19',                         desc: 'F19'},
    {id: 'F20 ',                        name: 'F20',                         desc: 'F20'},
    {id: 'F21 ',                        name: 'F21',                         desc: 'F21'},
    {id: 'F22 ',                        name: 'F22',                         desc: 'F22'},
    {id: 'F23 ',                        name: 'F23',                         desc: 'F23'},
    {id: 'F24 ',                        name: 'F24',                         desc: 'F24'},
    {id: 'EXECUTE',                     name: 'EXECUTE',                     desc: 'EXECUTE'},
    {id: 'HELP',                        name: 'HELP',                        desc: 'HELP'},
    {id: 'MENU',                        name: 'MENU',                        desc: 'MENU'},
    {id: 'SELECT',                      name: 'SELECT',                      desc: 'SELECT'},
    {id: 'STOP',                        name: 'STOP',                        desc: 'STOP'},
    {id: 'AGAIN',                       name: 'AGAIN',                       desc: 'AGAIN'},
    {id: 'UNDO',                        name: 'UNDO',                        desc: 'UNDO'},
    {id: 'CUT',                         name: 'CUT',                         desc: 'CUT'},
    {id: 'COPY',                        name: 'COPY',                        desc: 'COPY'},
    {id: 'PASTE',                       name: 'PASTE',                       desc: 'PASTE'},
    {id: 'FIND',                        name: 'FIND',                        desc: 'FIND'},
    {id: '_MUTE',                       name: '_MUTE',                       desc: '_MUTE(Not work on Windows)'},
    {id: '_VOLUP',                      name: '_VOLUP',                      desc: '_VOLUP(Not work on Windows)'},
    {id: '_VOLDOWN',                    name: '_VOLDOWN',                    desc: '_VOLDOWN(Not work on Windows)'},
    {id: 'LCAP',                        name: 'Locking Caps Lock',           desc: 'Locking Caps Lock'},
    {id: 'LNUM',                        name: 'Locking Num Lock',            desc: 'Locking Num Lock'},
    {id: 'LSCR',                        name: 'Locking Scroll Lock',         desc: 'Locking Scroll Lock'},
    {id: 'PCMM',                        name: 'P,',                          desc: 'Keypad Comma'},
    {id: 'KP_EQUAL_AS400',              name: 'P= (AS400)',                  desc: 'Keypad Equal (AS400)'},
    {id: 'INT1',                        name: '\u308D',                      desc: 'Japanese RO'},
    {id: 'INT2',                        name: '\u3072\u3089\u304c\u306a',    desc: 'Japanese Hiragana'},
    {id: 'INT3',                        name: '\uffe5',                      desc: 'Japanese Yen'},
    {id: 'INT4',                        name: '\u5909\u63db',                desc: 'Japanese Henkan'},
    {id: 'INT5',                        name: '\u7121\u5909\u63db',          desc: 'Japanese Muhenkan'},
    {id: 'INT6',                        name: 'INT6',                        desc: 'INT6'},
    {id: 'INT7',                        name: 'INT7',                        desc: 'INT7'},
    {id: 'INT8',                        name: 'INT8',                        desc: 'INT8'},
    {id: 'INT9',                        name: 'INT9',                        desc: 'INT9'},
    {id: 'LANG1',                       name: '\ud55c/\uc601',                       desc: 'Korean Hangul/English'},
    {id: 'LANG2',                       name: '\ud55c\uc790',                desc: 'Korean Hanja'},
    {id: 'LANG3',                       name: 'LANG3',                       desc: 'LANG3'},
    {id: 'LANG4',                       name: 'LANG4',                       desc: 'LANG4'},
    {id: 'LANG5',                       name: 'LANG5',                       desc: 'LANG5'},
    {id: 'LANG6',                       name: 'LANG6',                       desc: 'LANG6'},
    {id: 'LANG7',                       name: 'LANG7',                       desc: 'LANG7'},
    {id: 'LANG8',                       name: 'LANG8',                       desc: 'LANG8'},
    {id: 'LANG9',                       name: 'LANG9',                       desc: 'LANG9'},
    {id: 'ALT_ERASE',                   name: 'ALT_ERASE',                   desc: 'ALT_ERASE'},
    {id: 'SYSREQ',                      name: 'SYSREQ',                      desc: 'SYSREQ'},
    {id: 'CANCEL',                      name: 'CANCEL',                      desc: 'CANCEL'},
    {id: 'CLEAR',                       name: 'CLEAR',                       desc: 'CLEAR'},
    {id: 'PRIOR',                       name: 'PRIOR',                       desc: 'PRIOR'},
    {id: 'RETURN',                      name: 'RETURN',                      desc: 'RETURN'},
    {id: 'SEPARATOR',                   name: 'SEPARATOR',                   desc: 'SEPARATOR'},
    {id: 'OUT',                         name: 'OUT',                         desc: 'OUT'},
    {id: 'OPER',                        name: 'OPER',                        desc: 'OPER'},
    {id: 'CLEAR_AGAIN',                 name: 'CLEAR_AGAIN',                 desc: 'CLEAR_AGAIN'},
    {id: 'CRSEL',                       name: 'CRSEL',                       desc: 'CRSEL'},
    {id: 'EXSEL',                       name: 'EXSEL',                       desc: 'EXSEL'},

    /* System & Media key 165-191(0xa5-bf) */
    {id: 'PWR ',                        name: 'Sys Power',                   desc: 'System Power'},
    {id: 'SLEP',                        name: 'Sys Sleep',                   desc: 'System Sleep'},
    {id: 'WAKE',                        name: 'Sys Wake',                    desc: 'System Wake'},
    {id: 'MUTE',                        name: 'Mute',                        desc: 'Audio Mute'},
    {id: 'VOLU',                        name: 'Vol Up',                      desc: 'Audio Vol Up'},
    {id: 'VOLD',                        name: 'Vol Down',                    desc: 'Audio Vol Down'},
    {id: 'MNXT',                        name: 'Next Track',                  desc: 'Next Track'},
    {id: 'MPRV',                        name: 'Prev Track',                  desc: 'Previous Track'},
    {id: 'MSTP',                        name: 'Stop',                        desc: 'Media Stop'},
    {id: 'MPLY',                        name: 'Play Pause',                  desc: 'Play Pause'},
    {id: 'MSEL',                        name: 'Select',                      desc: 'Media Select'},
    {id: 'EJCT',                        name: 'Eject',                       desc: 'Media Eject'},
    {id: 'MAIL',                        name: 'Mail',                        desc: 'Mail'},
    {id: 'CALC',                        name: 'Calc',                        desc: 'Calculator'},
    {id: 'MYCM',                        name: 'My Computer',                 desc: 'My Computer'},
    {id: 'WSCH',                        name: 'Web Search',                  desc: 'WWW Search'},
    {id: 'WHOM',                        name: 'Web Home',                    desc: 'WWW Home'},
    {id: 'WBAK',                        name: 'Web Back',                    desc: 'WWW Back'},
    {id: 'WFWD',                        name: 'Web Forward',                 desc: 'WWW Forward'},
    {id: 'WSTP',                        name: 'Web Stop',                    desc: 'WWW Stop'},
    {id: 'WREF',                        name: 'Web Refresh',                 desc: 'WWW Refresh'},
    {id: 'WFAV',                        name: 'Web Favorites',               desc: 'WWW Favorites'},
    {id: 'RESERVED-187',                name: 'RESERVED-187',                desc: 'RESERVED-187'},
    {id: 'RESERVED-188',                name: 'RESERVED-188',                desc: 'RESERVED-188'},
    {id: 'RESERVED-189',                name: 'RESERVED-189',                desc: 'RESERVED-189'},
    {id: 'RESERVED-190',                name: 'RESERVED-190',                desc: 'RESERVED-190'},
    {id: 'RESERVED-191',                name: 'RESERVED-191',                desc: 'RESERVED-191'},

    /* Fn key 192-223(0xc0-df) */
    {id: 'FN0 ',                        name: 'L1',                          desc: 'Change to Layer 1(Momentary) '},
    {id: 'FN1 ',                        name: 'L2',                          desc: 'Change to Layer 2(Momentary) '},
    {id: 'FN2 ',                        name: 'L3',                          desc: 'Change to Layer 3(Momentary) '},
    {id: 'FN3 ',                        name: 'L4',                          desc: 'Change to Layer 4(Momentary) '},
    {id: 'FN4 ',                        name: 'L5',                          desc: 'Change to Layer 5(Momentary) '},
    {id: 'FN5 ',                        name: 'L6',                          desc: 'Change to Layer 6(Momentary) '},
    {id: 'FN6 ',                        name: 'L7',                          desc: 'Change to Layer 7(Momentary) '},
    {id: 'FN7 ',                        name: 'T1',                          desc: 'Change to Layer 1(Toggle) '},
    {id: 'FN8 ',                        name: 'T2',                          desc: 'Change to Layer 2(Toggle) '},
    {id: 'FN9 ',                        name: 'T3',                          desc: 'Change to Layer 3(Toggle) '},
    {id: 'FN10',                        name: 'L1t',                         desc: 'Change to Layer 1(Momentary with Tap Toggle) '},
    {id: 'FN11',                        name: 'L2t',                         desc: 'Change to Layer 2(Momentary with Tap Toggle) '},
    {id: 'FN12',                        name: 'L3t',                         desc: 'Change to Layer 3(Momentary with Tap Toggle) '},
    /* Layer swith with Tap key */
    {id: 'FN13',                        name: 'A (L1)',                      desc: 'A with with L1(Tap key)'},
    {id: 'FN14',                        name: 'F (L2)',                      desc: 'F with with L2(Tap key)'},
    {id: 'FN15',                        name: 'J (L3)',                      desc: 'J with with L3(Tap key)'},
    {id: 'FN16',                        name: 'Space (L4)',                  desc: 'Space with L4(Tap key)'},
    {id: 'FN17',                        name: '; (L5)',                      desc: 'Semicolon with L5(Tap key)'},
    {id: 'FN18',                        name: '\'( L6)',                     desc: 'Quote with L6(Tap key)'},
    {id: 'FN19',                        name: '/ (L7)',                      desc: 'Slash with with L7(Tap key)'},
    /* Modifier on alpha key(Tap key, Dual-role key) */
    {id: 'FN20',                        name: 'Space (LShift)',              desc: 'Space with Left Sfhit(Tap key)'},
    {id: 'FN21',                        name: 'Space (LCtrl)',               desc: 'Space with Left Control(Tap key)'},
    {id: 'FN22',                        name: '\' (RCtrl)',                  desc: 'Quote with Right Control(Tap key)'},
    {id: 'FN23',                        name: 'Enter (RCtrl)',               desc: 'Enter with Right Control(Tap key)'},
    /* Modifier with a key(Tap key, Dual-role key) */
    {id: 'FN24',                        name: 'LCtrl (Esc)',                 desc: 'Left Control with Escape'},
    {id: 'FN25',                        name: 'LCtrl (Backspace)',           desc: 'Left Control with Backspace'},
    {id: 'FN26',                        name: 'LCtrl (OneShot)',             desc: 'Left Control(OneShot Modifier)'},
    {id: 'FN27',                        name: 'LShift (Esc)',                desc: 'Left Shift with Escape'},
    {id: 'FN28',                        name: 'LShift (Backspace)',          desc: 'Left Shift with Backspace'},
    {id: 'FN29',                        name: 'LShift (OneShot)',            desc: 'Left Shift(OneShot Modifier)'},
    {id: 'FN30',                        name: 'RShift (`)',                  desc: 'Right Shift with Grave(Tap key)'},
    {id: 'FN31',                        name: 'RShift (\\)',                 desc: 'Right Shift with Backslash(Tap key)'},

/* Standeard codes for 16bit Action 165-223(0xa5-df)
    {id: 'RESERVED-165',                name: 'RESERVED-165',                desc: 'RESERVED-165'},
    {id: 'RESERVED-166',                name: 'RESERVED-166',                desc: 'RESERVED-166'},
    {id: 'RESERVED-167',                name: 'RESERVED-167',                desc: 'RESERVED-167'},
    {id: 'RESERVED-168',                name: 'RESERVED-168',                desc: 'RESERVED-168'},
    {id: 'RESERVED-169',                name: 'RESERVED-169',                desc: 'RESERVED-169'},
    {id: 'RESERVED-170',                name: 'RESERVED-170',                desc: 'RESERVED-170'},
    {id: 'RESERVED-171',                name: 'RESERVED-171',                desc: 'RESERVED-171'},
    {id: 'RESERVED-172',                name: 'RESERVED-172',                desc: 'RESERVED-172'},
    {id: 'RESERVED-173',                name: 'RESERVED-173',                desc: 'RESERVED-173'},
    {id: 'RESERVED-174',                name: 'RESERVED-174',                desc: 'RESERVED-174'},
    {id: 'RESERVED-175',                name: 'RESERVED-175',                desc: 'RESERVED-175'},
    {id: 'KP_00',                       name: 'KP_00',                       desc: 'KP_00'},
    {id: 'KP_000',                      name: 'KP_000',                      desc: 'KP_000'},
    {id: 'THOUSANDS_SEPARATOR',         name: 'THOUSANDS_SEPARATOR',         desc: 'THOUSANDS_SEPARATOR'},
    {id: 'DECIMAL_SEPARATOR',           name: 'DECIMAL_SEPARATOR',           desc: 'DECIMAL_SEPARATOR'},
    {id: 'CURRENCY_UNIT',               name: 'CURRENCY_UNIT',               desc: 'CURRENCY_UNIT'},
    {id: 'CURRENCY_SUB_UNIT',           name: 'CURRENCY_SUB_UNIT',           desc: 'CURRENCY_SUB_UNIT'},
    {id: 'KP_LPAREN',                   name: 'KP_LPAREN',                   desc: 'KP_LPAREN'},
    {id: 'KP_RPAREN',                   name: 'KP_RPAREN',                   desc: 'KP_RPAREN'},
    {id: 'KP_LCBRACKET',                name: 'KP_LCBRACKET',                desc: 'KP_LCBRACKET'},
    {id: 'KP_RCBRACKET',                name: 'KP_RCBRACKET',                desc: 'KP_RCBRACKET'},
    {id: 'KP_TAB',                      name: 'KP_TAB',                      desc: 'KP_TAB'},
    {id: 'KP_BSPACE',                   name: 'KP_BSPACE',                   desc: 'KP_BSPACE'},
    {id: 'KP_A',                        name: 'KP_A',                        desc: 'KP_A'},
    {id: 'KP_B',                        name: 'KP_B',                        desc: 'KP_B'},
    {id: 'KP_C',                        name: 'KP_C',                        desc: 'KP_C'},
    {id: 'KP_D',                        name: 'KP_D',                        desc: 'KP_D'},
    {id: 'KP_E',                        name: 'KP_E',                        desc: 'KP_E'},
    {id: 'KP_F',                        name: 'KP_F',                        desc: 'KP_F'},
    {id: 'KP_XOR',                      name: 'KP_XOR',                      desc: 'KP_XOR'},
    {id: 'KP_HAT',                      name: 'KP_HAT',                      desc: 'KP_HAT'},
    {id: 'KP_PERC',                     name: 'KP_PERC',                     desc: 'KP_PERC'},
    {id: 'KP_LT',                       name: 'KP_LT',                       desc: 'KP_LT'},
    {id: 'KP_GT',                       name: 'KP_GT',                       desc: 'KP_GT'},
    {id: 'KP_AND',                      name: 'KP_AND',                      desc: 'KP_AND'},
    {id: 'KP_LAZYAND',                  name: 'KP_LAZYAND',                  desc: 'KP_LAZYAND'},
    {id: 'KP_OR',                       name: 'KP_OR',                       desc: 'KP_OR'},
    {id: 'KP_LAZYOR',                   name: 'KP_LAZYOR',                   desc: 'KP_LAZYOR'},
    {id: 'KP_COLON',                    name: 'KP_COLON',                    desc: 'KP_COLON'},
    {id: 'KP_HASH',                     name: 'KP_HASH',                     desc: 'KP_HASH'},
    {id: 'KP_SPACE',                    name: 'KP_SPACE',                    desc: 'KP_SPACE'},
    {id: 'KP_ATMARK',                   name: 'KP_ATMARK',                   desc: 'KP_ATMARK'},
    {id: 'KP_EXCLAMATION',              name: 'KP_EXCLAMATION',              desc: 'KP_EXCLAMATION'},
    {id: 'KP_MEM_STORE',                name: 'KP_MEM_STORE',                desc: 'KP_MEM_STORE'},
    {id: 'KP_MEM_RECALL',               name: 'KP_MEM_RECALL',               desc: 'KP_MEM_RECALL'},
    {id: 'KP_MEM_CLEAR',                name: 'KP_MEM_CLEAR',                desc: 'KP_MEM_CLEAR'},
    {id: 'KP_MEM_ADD',                  name: 'KP_MEM_ADD',                  desc: 'KP_MEM_ADD'},
    {id: 'KP_MEM_SUB',                  name: 'KP_MEM_SUB',                  desc: 'KP_MEM_SUB'},
    {id: 'KP_MEM_MUL',                  name: 'KP_MEM_MUL',                  desc: 'KP_MEM_MUL'},
    {id: 'KP_MEM_DIV',                  name: 'KP_MEM_DIV',                  desc: 'KP_MEM_DIV'},
    {id: 'KP_PLUS_MINUS',               name: 'KP_PLUS_MINUS',               desc: 'KP_PLUS_MINUS'},
    {id: 'KP_CLEAR',                    name: 'KP_CLEAR',                    desc: 'KP_CLEAR'},
    {id: 'KP_CLEAR_ENTRY',              name: 'KP_CLEAR_ENTRY',              desc: 'KP_CLEAR_ENTRY'},
    {id: 'KP_BINARY',                   name: 'KP_BINARY',                   desc: 'KP_BINARY'},
    {id: 'KP_OCTAL',                    name: 'KP_OCTAL',                    desc: 'KP_OCTAL'},
    {id: 'KP_DECIMAL',                  name: 'KP_DECIMAL',                  desc: 'KP_DECIMAL'},
    {id: 'KP_HEXADECIMAL',              name: 'KP_HEXADECIMAL',              desc: 'KP_HEXADECIMAL'},
    {id: 'RESERVED-222',                name: 'RESERVED-222',                desc: 'RESERVED-222'},
    {id: 'RESERVED-223',                name: 'RESERVED-223',                desc: 'RESERVED-223'},
*/

    /* Modifier 224-231(0xe0-e7) */
    {id: 'LCTL',                        name: 'LCtrl',                       desc: 'Left Control'},
    {id: 'LSFT',                        name: 'LShift',                      desc: 'Left Shift'},
    {id: 'LALT',                        name: 'LAlt',                        desc: 'Left Alt(\u2325)'},
    {id: 'LGUI',                        name: 'LGui',                        desc: 'Left Windows(\u2318)'},
    {id: 'RCTL',                        name: 'RCtrl',                       desc: 'Right Control'},
    {id: 'RSFT',                        name: 'RShift',                      desc: 'Right Shift'},
    {id: 'RALT',                        name: 'RAlt',                        desc: 'Right Alt(\u2325)'},
    {id: 'RGUI',                        name: 'RGui',                        desc: 'Right Windows(\u2318)'},

    /* Not used 232-239(0xe8-ef) */
    {id: 'RESERVED-232',                name: 'RESERVED-232',                desc: 'RESERVED-232'},
    {id: 'RESERVED-233',                name: 'RESERVED-233',                desc: 'RESERVED-233'},
    {id: 'RESERVED-234',                name: 'RESERVED-234',                desc: 'RESERVED-234'},
    {id: 'RESERVED-235',                name: 'RESERVED-235',                desc: 'RESERVED-235'},
    {id: 'RESERVED-236',                name: 'RESERVED-236',                desc: 'RESERVED-236'},
    {id: 'RESERVED-237',                name: 'RESERVED-237',                desc: 'RESERVED-237'},
    {id: 'RESERVED-238',                name: 'RESERVED-238',                desc: 'RESERVED-238'},
    {id: 'RESERVED-239',                name: 'RESERVED-239',                desc: 'RESERVED-239'},

    /* Mousekey 240-255(0xf0-ff) */
    {id: 'MS_U',                        name: 'Mouse Up',                    desc: 'Mouse UP'},
    {id: 'MS_D',                        name: 'Mouse down',                  desc: 'Mouse Down'},
    {id: 'MS_L',                        name: 'Mouse left',                  desc: 'Mouse Left'},
    {id: 'MS_R',                        name: 'Mouse right',                 desc: 'Mouse Right'},
    {id: 'BTN1',                        name: 'Mouse Btn1',                  desc: 'Mouse Button1'},
    {id: 'BTN2',                        name: 'Mouse Btn2',                  desc: 'Mouse Button2'},
    {id: 'BTN3',                        name: 'Mouse Btn3',                  desc: 'Mouse Button3'},
    {id: 'BTN4',                        name: 'Mouse Btn4',                  desc: 'Mouse Button4'},
    {id: 'BTN5',                        name: 'Mouse Btn5',                  desc: 'Mouse Button5'},
    {id: 'WH_U',                        name: 'Wheel Up',                    desc: 'Wheel Up'},
    {id: 'WH_D',                        name: 'Wheel Down',                  desc: 'Wheel Down'},
    {id: 'WH_L',                        name: 'Wheel Left',                  desc: 'Wheel Left'},
    {id: 'WH_R',                        name: 'Wheel Right',                 desc: 'Wheel Right'},
    {id: 'ACL0',                        name: 'Mouse Slow',                  desc: 'Mouse Slow'},
    {id: 'ACL1',                        name: 'Mouse Medium',                desc: 'Mouse Medium'},
    {id: 'FN',                          name: 'Fn',                          desc: 'Alternative Function'},
];
