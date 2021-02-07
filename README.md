# Ultra-Fast Traceroute
_Creators:_   
_Yuvraj Singh Malhi (BITS Pilani Electronics)_   
_Shivam Goyal       (BITS Pilani Computers)_   

## Description
Traceroute is a very famous networking tools to determine the path from a machine to another machine. 

Traditionally, a traceroute would use the ICMP protocol which the routers understand. In every second, 3 ICMP packets are sent to the destination with increasing _TTL_ (time to live) starting from 1, then 2, then 3.. and so on.

_For TTL = 1_ : The most immediate router from the host to the destination is found.
_For TTL = 2_ : The second most immediate router from host to destination is found. 
_For TTL = 3_ : The third most immediate router from host to destination is found. 
.. and so on.

This goes on till the destination is reached. Hence, all the routers in the path have been identified.

![image](https://user-images.githubusercontent.com/76866159/107150588-74234480-6984-11eb-859f-a9e297473a3e.png)

In linux,
the command to use traceroute for _google.com_  would be:   
``` traceroute google.com ```   

![image](https://user-images.githubusercontent.com/76866159/107150696-1f33fe00-6985-11eb-9ed6-fd3d32b7828b.png)



