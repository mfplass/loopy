cat << EOF
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>Loopy simulator</title>
</head>
<body onload="initregs()">
<script src="uhbfbqewr.js"></script>
<noscript><div>Please enable javascript in your browser.</div></noscript>
<svg width="256" height="340">
<rect id="case" width="234" height="310"
     x="20" y="20" fill="black" />
EOF
G () {
  echo '<g id="'$group'">'
  x=40
  y=$((40+16*group+4*(group/4)))
  i=0
  while [ $i -lt 16 ]; do
    echo '<circle id="r'$group'led'$((15-i))'" '
    echo ' fill="red" r="4.5" stroke_width="2" stroke="gray" '
    echo ' cx="'$x'" cy="'$y'" onclick="changeled(evt)" />'
    i=$((i+1))
    x=$((x+12+4*((i % 4)==0)))
  done
  echo '</g>'
  group=$((group+1))
}
group=0
G;G;G;G
G;G;G;G
G;G;G;G
G;G;G;G
G
cat << EOF
</svg>
<div>
  <pre id="counter">         0</pre>
  <button onclick="singlestep()">Step</button>
  <button id="slow" onclick="togglerun()">Slow</button>
  <button onclick="dump()">Dump</button>
</div>
<div id="dump">--</dump>
</body>					
</html>
