## PROTO

The PROTO target is for modifying the protocol number in the header of IP packets to avoid protocol identifying.

For IPv4 packets, the `Protocol` field is modified and the checksum is re-calculated.

For IPv6 packets, the scenario can be more complex due to the introduction of the extension headers mechanism. By default, the PROTO target will scan the IPv6 packet, finding the last extension header and modify its `Next-header` field. Normally, the following headers will be seen as an extension header:

* NEXTHDR_HOP
* NEXTHDR_ROUTING
* NEXTHDR_FRAGMENT
* NEXTHDR_AUTH
* NEXTHDR_DEST

For fragmented packets, only the first fragment is processed and other fragments are not touched. 

### `--proto-set` __`protocol`__

This option is mandatory. __`protocol`__ is the protocol number to which you want to modify the packets.

### `--stop-at-frag`

This option is only valid in `ip6tables`. When specifying this option, the fragment extension header will be seen as a non-extension header. 

### `--stop-at-auth`
This option is only valid in `ip6tables`. When specifying this option, the authentication extension header will be seen as a non-extension header.

