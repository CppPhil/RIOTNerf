Beim Start-Up:

----------------------------------------------
tmux
Strg + B und " = Split Window Horizontal
Strg + B und % = Split Window Vertical
Strg + B und Pfeiltaste = Navigation der geteilten Windows innerhalb eines Windows

vi
:u = reverse change
o = new line
dd = delete line
x = delete current elem

-----------------------------------------------
0. Ist die Addresse des IoT Ger�ts im index.js Code gesetzt? Oder ist es eine alte?
1. sudo radvd -n -d=5
2. node index.js (/home/pi)

Radvd -> Router, IoT ger�t bekommt global Ip
radvd.conf (/etc) -> fd1d:8d5c:12a5::/64 Entspricht eigene Adresse zus�tzlich ein Routingeintrag
iwpan phy phy0 set channel 0 26 -> Channel 26
sudo ip address add fd1d:8d5c:12a5::1/64 dev lowpan0
Routing -> Interface (fd1d:...) wird lowpan0 zugewiesen
check -> ip a -> Interface / Addresse vorhanden?
check -> ip -6 r -> routing vorhanden? *Interface* dev lowpan0