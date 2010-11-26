<map version="0.8.1">
<!-- To view this file, download free mind mapping software FreeMind from http://freemind.sourceforge.net -->
<node CREATED="1290774202826" ID="Freemind_Link_1320245527" MODIFIED="1290780523992" TEXT="Navilink protocol" VGAP="0">
<cloud/>
<font NAME="SansSerif" SIZE="12"/>
<hook NAME="accessories/plugins/NodeNote.properties">
<text>More information avalaible on http://doku.splitbrain.com/navilink/</text>
</hook>
<node CREATED="1290774383714" HGAP="35" ID="_" MODIFIED="1290780215952" POSITION="right" TEXT="Packet" VGAP="0" VSHIFT="5">
<font NAME="SansSerif" SIZE="12"/>
<icon BUILTIN="desktop_new"/>
<node CREATED="1290774401261" FOLDED="true" ID="Freemind_Link_1889017609" MODIFIED="1290780151000" TEXT="Start sequence" VSHIFT="6746">
<font NAME="SansSerif" SIZE="12"/>
<icon BUILTIN="full-1"/>
<node CREATED="1290774442573" ID="Freemind_Link_136488753" MODIFIED="1290780227671" TEXT="0xa0">
<font NAME="SansSerif" SIZE="12"/>
<icon BUILTIN="full-1"/>
</node>
<node CREATED="1290774449432" ID="Freemind_Link_1761277843" MODIFIED="1290780229186" TEXT="0xa2">
<font NAME="SansSerif" SIZE="12"/>
<icon BUILTIN="full-2"/>
</node>
</node>
<node CREATED="1290780679532" ID="Freemind_Link_659790960" MODIFIED="1290780730671" TEXT="Payload Packet Length (15-bits)">
<icon BUILTIN="full-2"/>
<node CREATED="1290780707154" ID="Freemind_Link_1937628668" MODIFIED="1290780720802" TEXT="Low byte">
<icon BUILTIN="full-1"/>
</node>
<node CREATED="1290780712499" ID="Freemind_Link_852809501" MODIFIED="1290780722026" TEXT="High byte">
<icon BUILTIN="full-2"/>
</node>
</node>
<node CREATED="1290774418292" ID="Freemind_Link_1069689544" MODIFIED="1290780165562" TEXT="Payload packet (Up to 2^15 -1)" VGAP="0" VSHIFT="19">
<font NAME="SansSerif" SIZE="12"/>
<icon BUILTIN="full-3"/>
<node CREATED="1290774513228" ID="Freemind_Link_1655132050" MODIFIED="1290780239748" TEXT="Packet ID" VGAP="0" VSHIFT="2364">
<font NAME="SansSerif" SIZE="12"/>
<icon BUILTIN="full-1"/>
<node CREATED="1290774810974" ID="Freemind_Link_847988709" MODIFIED="1290780459168" TEXT="PID_SYNC" VSHIFT="190">
<icon BUILTIN="Mail"/>
<node CREATED="1290775291015" ID="Freemind_Link_1844590394" MODIFIED="1290776390423" TEXT="Value: 0xd6&#xa;Sens: H-&gt;D&#xa;Payload: 1byte&#xa;Example: [A0 A2 01 00 d6 d6 00 B0 B3]&#xa;Description: The beginning packet to check if NAVi GPS device is ready or not.&#xa;After: Expect to receive PID_ACK if NAViGPS is ready. The result can be failed if NAViGPS is not in link mode or USB connection is not ready." VSHIFT="81"/>
</node>
<node CREATED="1290774843068" ID="Freemind_Link_527736668" MODIFIED="1290780459168" TEXT="PID_ACK" VSHIFT="1">
<icon BUILTIN="Mail"/>
<node CREATED="1290775291015" ID="Freemind_Link_790301916" MODIFIED="1290776390423" TEXT="Value: 0x0c&#xa;Sens: H&lt;-&gt;D&#xa;Payload: 1 byte&#xa;Example: [A0 A2 01 00 0c 0c 00 B0 B3]&#xa;Description: General acknowledge packet" VSHIFT="61"/>
</node>
<node CREATED="1290774848208" ID="Freemind_Link_857243508" MODIFIED="1290780459168" TEXT="PID_NACK" VSHIFT="-4">
<icon BUILTIN="Mail"/>
<node CREATED="1290775291015" ID="Freemind_Link_1342656432" MODIFIED="1290776390407" TEXT="Value: 0x00&#xa;Sens: H&lt;-&gt;D&#xa;Payload: 1 byte&#xa;Example: [A0 A2 01 00 00 00 00 B0 B3]&#xa;Description: General none-acknowledge packet" VSHIFT="51"/>
</node>
<node CREATED="1290774848208" ID="Freemind_Link_334634564" MODIFIED="1290780459168" TEXT="PID_QRY_INFORMATION" VSHIFT="-6">
<icon BUILTIN="Mail"/>
<node CREATED="1290775291015" ID="Freemind_Link_1060789971" MODIFIED="1290776390407" TEXT="Value: 0x20&#xa;Sens: H-&gt;D&#xa;Payload: 1 byte&#xa;Example: [A0 A2 01 00 20 20 00 B0 B3]&#xa;Description: Packet to query NAViGPS information&#xa;After: Expect to receive a PID_DATA packet with payload data in T_INFORMATION type" VSHIFT="63"/>
</node>
<node CREATED="1290774848208" ID="Freemind_Link_1177810616" MODIFIED="1290780459183" TEXT="PID_QRY_FW_VERSION" VSHIFT="1">
<icon BUILTIN="Mail"/>
<node CREATED="1290775291015" ID="Freemind_Link_1847381566" MODIFIED="1290776390407" TEXT="Value: 0xFE&#xa;Sens: H-&gt;D&#xa;Payload: 1 byte&#xa;Example: [A0 A2 01 00 FE FE 00 B0 B3]&#xa;Description: Packet to query the firmware version of the device&#xa;After: The device will return a PID_DATA packet holding the firmware version as a string." VSHIFT="63"/>
</node>
<node CREATED="1290774848208" ID="Freemind_Link_1628551761" MODIFIED="1290780459183" TEXT="PID_DATA" VSHIFT="1">
<icon BUILTIN="Mail"/>
<node CREATED="1290775291015" ID="Freemind_Link_1891514601" MODIFIED="1290776891244" TEXT="Value: 0x03&#xa;Sens: H&lt;-&gt;D&#xa;Payload: Up to 2^15 - 1 bytes&#xa;Description: General data packet." VSHIFT="45"/>
</node>
<node CREATED="1290774848208" ID="Freemind_Link_730212337" MODIFIED="1290780459183" TEXT="PID_ADD_A_WAYPOINT" VSHIFT="-2">
<icon BUILTIN="Mail"/>
<node CREATED="1290775291015" ID="Freemind_Link_1137593224" MODIFIED="1290777144647" TEXT="Value: 0x3c&#xa;Sens: H-&gt;D&#xa;Payload: 33 bytes&#xa;Byte 1..32 of following payload: waypoint data in T_WAYPOINT type&#xa;Description: Packet to add a route to NAViGPS&#xa;After: Expect to receive a PID_DATA packet with assigned waypoint ID if successful else PID_NAK is received" VSHIFT="58"/>
</node>
<node CREATED="1290774848208" ID="Freemind_Link_1660980584" MODIFIED="1290780459183" TEXT="PID_QRY_WAYPOINTS" VSHIFT="3">
<icon BUILTIN="Mail"/>
<node CREATED="1290775291015" ID="Freemind_Link_1764633353" MODIFIED="1290777819529" TEXT="Value:0x28&#xa;Sens: H-&gt;D&#xa;Payload: 8 bytes&#xa;Byte 1..4: the first waypoint to query by this packet, 0 based, waypoints are sorted by name&#xa;Byte 5..6: number of waypoints to query (1..32)&#xa;Byte 7: 0x01&#xa;After: Expect to receive PID_DATA packet with payload in T_WAYPOINTS, receive PID_NAK if no waypoints&#xa;read.&#xa;Example: to read the first waypoint by sending: [A0 A2 08 00 28 00 00 00 00 01 00 01 2A 00 B0 B3]&#xa;To read the second and third waypoints by sending: [A0 A2 08 00 28 01 00 00 00 02 00 01 2C 00 B0 B3]&#xa;Description:Packet to read 1 to 32 waypoints from NAVIGPS" VSHIFT="48"/>
</node>
<node CREATED="1290777264427" ID="Freemind_Link_757216316" MODIFIED="1290780459183" TEXT="PID_QRY_ROUTE" VSHIFT="-1">
<icon BUILTIN="Mail"/>
<node CREATED="1290777272599" ID="Freemind_Link_221376986" MODIFIED="1290777803061" TEXT="Value:0x24&#xa;Sens: H-&gt;D&#xa;Payload: 8 bytes&#xa;Byte 1..4: route number, 0..19, routes are sorted by name&#xa;Byte 5..6:0x0000&#xa;Byte 7: 0x01&#xa;After : Expect to receive PID_DATA with payload in T_ROUTE type&#xa;Example: to query the first route by sending: [A0 A2 08 00 24 00 00 00 00 00 00 01 25 00 B0 B3]&#xa;Description:Packet to query a route from NAViGPS"/>
</node>
<node CREATED="1290777429081" ID="Freemind_Link_115149240" MODIFIED="1290780459183" TEXT="PID_DEL_WAYPOINT" VSHIFT="3">
<icon BUILTIN="Mail"/>
<node CREATED="1290777272599" ID="Freemind_Link_451018970" MODIFIED="1290777907606" TEXT="Value:0x36&#xa;Sens: H-&gt;D&#xa;Payload: 5 bytes&#xa;Byte 1..2:0x0000&#xa;Byte 3..4:waypoint ID(0..499)&#xa;After :Expect to receive PID_ACK if successful, else PID_NAK is received. The waypoint used by any routes cannot be deleted.&#xa;Example: to delete a waypoint with ID 01 by sending: [A0 A2 05 00 36 00 00 01 00 37 00 B0 B3]&#xa;Description:Packet to delete one waypoint"/>
</node>
<node CREATED="1290777429081" ID="Freemind_Link_1359759233" MODIFIED="1290780459183" TEXT="PID_DEL_ALL_WAYPOINT" VSHIFT="1">
<icon BUILTIN="Mail"/>
<node CREATED="1290777272599" ID="Freemind_Link_1342681012" MODIFIED="1290777901185" TEXT="Value:0x37&#xa;Sens: H-&gt;D&#xa;Payload: 5 bytes&#xa;Byte 1..4:0x00f00000&#xa;After :Expect to receive PID_ACK if successful else PID_NAK is received. You have to delete all routes before deleting all waypoints.&#xa;Example: to delete all routes by sending: [A0 A2 05 00 37 00 00 f0 00 27 01 B0 B3]&#xa;Description:Packet request deleting all waypoints"/>
</node>
<node CREATED="1290777429081" ID="Freemind_Link_584895176" MODIFIED="1290780459183" TEXT="PID_DEL_ROUTE" VSHIFT="2">
<icon BUILTIN="Mail"/>
<node CREATED="1290777272599" ID="Freemind_Link_150436121" MODIFIED="1290778965234" TEXT="Value:0x34&#xa;Sens: H-&gt;D&#xa;Payload: 5 bytes&#xa;Byte 1..2:0x0000&#xa;Byte 3..4:route ID(0..19)&#xa;After :Expect to receive PID_ACK if successful, else PID_NAK is received.&#xa;Example: to delete all routes by sending: [A0 A2 05 00 37 00 00 f0 00 27 01 B0 B3]&#xa;Description:Packet to delete one route"/>
</node>
<node CREATED="1290777429081" ID="Freemind_Link_408431396" MODIFIED="1290780459183" TEXT="PID_DEL_ALL_ROUTE" VSHIFT="3">
<icon BUILTIN="Mail"/>
<node CREATED="1290777272599" ID="Freemind_Link_903551595" MODIFIED="1290779040295" TEXT="Value:0x35&#xa;Sens: H-&gt;D&#xa;Payload: 5 bytes&#xa;Byte 1..4:0x00ff0000&#xa;After :Expect to receive PID_ACK if successful, else PID_NAK is received.&#xa;Example:to delete all routes by sending [A0 A2 05 00 35 00 00 f0 00 25 01 B0 B3]&#xa;Description:Packet request deleting all routes"/>
</node>
<node CREATED="1290777429081" ID="Freemind_Link_493020813" MODIFIED="1290780459183" TEXT="PID_ADD_A_ROUTE" VSHIFT="-2">
<icon BUILTIN="Mail"/>
<node CREATED="1290777272599" ID="Freemind_Link_990852352" MODIFIED="1290779118013" TEXT="Value:0x3D&#xa;Sens: H-&gt;D&#xa;Payload: 1 + actual route length&#xa;Byte 1..n:route data in T_ROUTE type&#xa;After :Expect to receive PID_ACK if successful, else PID_NAK is received.&#xa;Description: Packet to add a route to NAViGPS"/>
</node>
<node CREATED="1290777429081" ID="Freemind_Link_1399926712" MODIFIED="1290780459183" TEXT="PID_ERASE_TRACK">
<icon BUILTIN="Mail"/>
<node CREATED="1290777272599" ID="Freemind_Link_775067563" MODIFIED="1290779600132" TEXT="Value:0x11&#xa;Sens: H-&gt;D&#xa;Payload: 5 bytes&#xa;Byte 1..4:track buffer address, typical value is 0x400e0000, value can be found in&#xa;T_INFORMATION.pTrackBuf &#xa;Byte 5..6: 0x0000&#xa;Byte 7 : always 0x00&#xa;After: Expect PID_CMD_OK in 3 seconds if successful.&#xa;Example: [A0 A2 08 00 11 00 00 0e 40 00 00 00 5F 00 B0 B3]&#xa;Description: Packet request deleting track&#xa;&#xa;"/>
</node>
<node CREATED="1290777429081" ID="Freemind_Link_1157053305" MODIFIED="1290780459183" TEXT="PID_READ_TRACKPOINTS" VSHIFT="-3">
<icon BUILTIN="Mail"/>
<node CREATED="1290777272599" ID="Freemind_Link_1554496207" MODIFIED="1290779741755" TEXT="Value:0x14&#xa;Sens: H-&gt;D&#xa;Payload: 8 bytes&#xa;Byte 1..4: start address (track buffer address+ offset)&#xa;Byte 5..6: data length to read (32 ..512*32)&#xa;Byte 7: always 0x00&#xa;After:Expect to receive PID_DATA with request track data in T_TRACKPOINT type in 4 seconds if successful. Send PID_ACK if data are received correctly.&#xa;Example: example, to read a track of 2561) points, 2 packets of PID_READ_TRACKPOINTS are needed. The&#xa;start address and byte length are as follows:&#xa;&#xa;------------------------------------------------&#xa;| Start address                 | Length      |&#xa;------------------------------------------------&#xa;|(0x400e0000+0)            |  32*256      |&#xa;|(0x400e0000+32*256)   | 32*(256-2) |&#xa;------------------------------------------------&#xa;Description: Packet request reading track logs from NAViGPS"/>
</node>
<node CREATED="1290777429081" ID="Freemind_Link_1972735674" MODIFIED="1290780459183" TEXT="PID_WRITE_TRACKPOINTS" VSHIFT="5">
<icon BUILTIN="Mail"/>
<node CREATED="1290777272599" ID="Freemind_Link_1830835946" MODIFIED="1290779831535" TEXT="Value:0x16&#xa;Sens: H-&gt;D&#xa;Payload: 8 bytes&#xa;Byte 1..4: start address(track buffer address + offset) typical value is 0x400e0000&#xa;Byte 5..6: data length to write (32 ..127*32)&#xa;Byte 7: always 0x00&#xa;After:After sending PID_WRITE_TRACKPOINTS packet, wait a short while (about 10ms seems to be enough) then send a PID_DATA packet with trackpoint data. Then you can expect to receive PID_CMD_OK within 4 seconds if successful. The maximum track points in one PID_DATA packet is 127. For example, to write a track of 520 points, 5 packets of PID_WRITE_TRACKPOINTS are needed.&#xa;&#xa;Description: Packet request to write track points to NAVI"/>
</node>
<node CREATED="1290777429081" ID="Freemind_Link_1140373030" MODIFIED="1290780459183" TEXT="PID_CMD_OK" VSHIFT="-2">
<icon BUILTIN="Mail"/>
<node CREATED="1290777272599" ID="Freemind_Link_1999066856" MODIFIED="1290779949737" TEXT="Value:0xf3&#xa;Sens: H&lt;-D&#xa;Payload: 1 bytes&#xa;Example:[A0 A2 01 00 f3 f3 00 B0 B3]&#xa;Description: Packet to indicate command is OK"/>
</node>
<node CREATED="1290777429081" ID="Freemind_Link_1081088949" MODIFIED="1290780459183" TEXT="PID_CMD_FAIL" VSHIFT="-3">
<icon BUILTIN="Mail"/>
<node CREATED="1290777272599" ID="Freemind_Link_933222080" MODIFIED="1290779942815" TEXT="Value:0xf4&#xa;Sens: H&lt;-D&#xa;Payload: 1 bytes&#xa;Example:[A0 A2 01 00 f4 f4 00 B0 B3]&#xa;Description: Packet to indicate command failed"/>
</node>
<node CREATED="1290777429081" ID="Freemind_Link_620162630" MODIFIED="1290780459183" TEXT="PID_QUIT" VSHIFT="-115">
<icon BUILTIN="Mail"/>
<node CREATED="1290777272599" ID="Freemind_Link_1634576104" MODIFIED="1290779989970" TEXT="Value:0xf2&#xa;Sens: H-&gt;D&#xa;Payload: 1 bytes&#xa;Example:[A0 A2 01 00 f2 f2 00 B0 B3]&#xa;Description: Packet to end connection."/>
</node>
</node>
<node CREATED="1290774573180" ID="Freemind_Link_1397006554" MODIFIED="1290780242342" TEXT="Lowest byte of payload data" VSHIFT="54">
<font NAME="SansSerif" SIZE="12"/>
<icon BUILTIN="full-2"/>
</node>
<node CREATED="1290774576805" ID="Freemind_Link_159291989" MODIFIED="1290780246592" TEXT="......" VSHIFT="2">
<font NAME="SansSerif" SIZE="12"/>
<icon BUILTIN="help"/>
</node>
<node CREATED="1290774580290" HGAP="23" ID="Freemind_Link_832950892" MODIFIED="1290780249030" TEXT="Highest of payload data" VSHIFT="-2304">
<font NAME="SansSerif" SIZE="12"/>
<icon BUILTIN="help"/>
</node>
</node>
<node CREATED="1290774425151" ID="Freemind_Link_1003502160" MODIFIED="1290780167124" TEXT="Checksum of the payload packet(15-bits)" VSHIFT="38">
<font NAME="SansSerif" SIZE="12"/>
<icon BUILTIN="full-4"/>
<node CREATED="1290774737835" ID="Freemind_Link_1681440027" MODIFIED="1290780254639" TEXT="Low byte" VSHIFT="56">
<font NAME="SansSerif" SIZE="12"/>
<icon BUILTIN="full-1"/>
</node>
<node CREATED="1290774751459" ID="Freemind_Link_721008377" MODIFIED="1290780256217" TEXT="High byte" VSHIFT="-37">
<font NAME="SansSerif" SIZE="12"/>
<icon BUILTIN="full-2"/>
</node>
<node CREATED="1290775155079" ID="Freemind_Link_1147155010" MODIFIED="1290780278889" TEXT="Algorithm:&#xa;checksum = 0&#xa;for byte in payload_packet:&#xa;&#x9;checksum= checksum + byte&#xa;cheksum = checksum AND 0x7fff" VSHIFT="-26">
<font NAME="SansSerif" SIZE="12"/>
<icon BUILTIN="ksmiletris"/>
</node>
</node>
<node CREATED="1290774432588" ID="Freemind_Link_1581155302" MODIFIED="1290780168734" TEXT="End sequence" VSHIFT="-6791">
<font NAME="SansSerif" SIZE="12"/>
<icon BUILTIN="full-5"/>
<node CREATED="1290774764537" ID="Freemind_Link_1560772142" MODIFIED="1290780286123" TEXT="0xb0" VSHIFT="6">
<font NAME="SansSerif" SIZE="12"/>
<icon BUILTIN="full-1"/>
</node>
<node CREATED="1290774768475" ID="Freemind_Link_1472466856" MODIFIED="1290780287701" TEXT="0xb3">
<font NAME="SansSerif" SIZE="12"/>
<icon BUILTIN="full-2"/>
</node>
</node>
</node>
<node CREATED="1290780506130" HGAP="36" ID="Freemind_Link_1099400112" MODIFIED="1290780531568" POSITION="right" TEXT="Data Structures">
<icon BUILTIN="penguin"/>
</node>
</node>
</map>
