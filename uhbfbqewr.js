var reg = [0,0,0,0, 34464, 61350, 43177, 42918,8,9,10,11,0,0,14,0,0];
var mem = [
    0x10fc,
    0x10ff,
    26,
    0xda,
    0x6ff,
    0x1000,	// r0 += 1 
    0x2000, // r0 += 2
    0xf011, // r1 += -1
    0x10fc, // load data address into MAR
    0x20cf, // jump around data
    0x1234, // data
    0x5678, // more data
    0x00db, // load first data word into rB
    0x00da, // also into rA
    0x0602, // r2 = r0 + rA
    0x0503, // r3 = r0 + (-1 - rB)
    0x1504, // r4 = r0 + (-1 - rB) + 1 
    0x10cc, // increment MAR
    0x004d, // store into second data word
    0x0E45, // save carry of rA+r4 in r5
    0x15b8, // clear r8
    0x10fc, // inline data address
    0x10cf, // jump around it
    0x0006, // data
    0x00d6, // r6 = 6 (r6 will be the counter)
    0xf86b, // rB = (r6 == 0) ? 0 : ~0
    0x31ff, // jump to .+3 if false
    0x7088, // r8 = r8 + 7
    0xf066, // r6 = r6 - 1
    0xb0ff, // loop back to .-5
    0x05bf, // halt
    0, 0, 0,
    0x00fe,
    0x4a, 0xeab, 0x16aa, 0xf1a4,
    0x5a, 0xeab, 0x16aa, 0xf1a5,
    0x6a, 0xeab, 0x16aa, 0xf1a6,
    0x7a, 0xeab, 0x16aa, 0xf1a7,
    0,0,0,0,0x00ef
];
var stats = {count: 0};

function showled(target) {
    if (target.reg[target.regnum] & (1 << target.regbit))
        target.setAttributeNS(null, "fill", target.litcolor);
    else
        target.setAttributeNS(null, "fill", "black");
}

function showreg(r) {
    for (j = 0; j < 16; j++) {
        var t = document.getElementById("r"+r+"led"+j);
        if (t) showled(t);
    }
}

function regchanged(r) {
    showreg(r);
    if (r == 0xC) {
        reg[0xD] = mem[reg[0xC]] || 0;
        showreg(0xD);
    }
    else if (r == 0xD) {
        mem[reg[0xC]] = reg[0xD];
        if (reg[0xC] == reg[0xF]) {
            reg[16] = reg[0xD];
            showreg(16);
        }
    }
    else if (r == 0xF) {
        reg[16] = mem[reg[0xF]] || 0;
        showreg(16);
    }
    else if (r == 16) {
        mem[reg[0xF]] = reg[16];
        if (reg[0xF] == reg[0xC]) {
            reg[0xD] = reg[16];
            showreg(0xD);
        }
    }
}

function initregs() {
    var i, j;
    for (i = 0; i <= 16; i++) {
        for (j = 0; j < 16; j++) {
            var t = document.getElementById("r"+i+"led"+j);
            if (t) {
                t.reg = reg;
                t.regnum = i;
                t.regbit = j;
                t.litcolor = (i == 16) ? "green" : "red";
            }
            else
                console.log(" oops "+"r"+i+"led"+j);
        }
    }
    for (i = 0; i < 16; i++)
        regchanged(i);
    stats.element = document.getElementById("counter");
}

function chstate(e, a) {
    if (!e.target.reg)
        initregs();
    e.target.reg[e.target.regnum] ^= 1 << e.target.regbit;
    regchanged(e.target.regnum);
}

function singlestep() {
    var pc, ir, d, s, imm, x, cy, y, t;
    pc = reg[0xF];
    ir = mem[pc] || 0;
    d = ir & 0xF;       /* destination register */
    s = (ir >> 4) & 0xF;/* source register */
    if (pc >= 0xFFF0)
        return;
    if ((s == 0xD || d == 0xD) && reg[0xC] >= 0xFFF0)
        return;
    pc = pc + 1;        /* increment program counter */
    reg[0xF] = pc;
    regchanged(0xF);
    imm = ir >> 12;
    if (imm > 7)
        imm += 0xFFF0;  /* sign-extend immediate value */
    x = 0xFFFF;         /* initial value */
    if (ir & 0x200)
        x &= reg[0xA];	/* conditional AND with A */
    if (ir & 0x100)
        x &= reg[0xB] ^ 0xFFFF;	/* conditional AND with NOT B */
    cy = 0;
    if (ir & 0x400)
        cy = imm & 1;   /* use only low-order bit of imm in this case */
    else
        x = (x & 0x8000) ? imm : 0;
    y = (s == 0xF) ? pc : (s == 0xD) ? (mem[reg[0xC]] || 0) : reg[s];
    t = x + y + cy;     /* adder at work */
    if (ir & 0x800)	/* result is shifted right */
        t = (t >> 1);
    else
        t &= 0xFFFF;
    reg[d] = t;
    regchanged(d);
    stats.count++;
    stats.element.textContent = ("         " + stats.count).substr(-10);
}

function slowrun() {
    if (!stats.running)
        return;
    var c = stats.count;
    singlestep();
    if (c != stats.count)
        setTimeout("slowrun()", 10);
    else
        togglerun();
}

function togglerun() {
    var b = document.getElementById("slow");
    stats.running = !stats.running;
    if (stats.running) {
        b.textContent = "Stop";
        slowrun();
    }
    else
        b.textContent = "Slow";
}

function dump() {
    var t = document.getElementById("dump");
    t.textContent = "" + mem;
}

function changeled(evt) {
    chstate(evt, 1);
}
