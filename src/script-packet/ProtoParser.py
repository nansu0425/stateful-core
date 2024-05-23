class ProtoParser():
    def __init__(self, start_id, recv_prefix, send_prefix):
        self.recv_pkts = []
        self.send_pkts = []
        self.total_pkts = []
        self.start_id = start_id
        self.cur_id = start_id
        self.recv_prefix = recv_prefix
        self.send_prefix = send_prefix
        
    def parse_proto(self, path):
        proto_file = open(path, 'r')
        lines = proto_file.readlines()
        
        for line in lines:
            if line.startswith('message') == False:
                continue
            
            pkt_name = line.split()[1].upper()
            if (pkt_name.startswith(self.recv_prefix)):
                self.recv_pkts.append(Packet(pkt_name, self.cur_id))
            elif (pkt_name.startswith(self.send_prefix)):
                self.send_pkts.append(Packet(pkt_name, self.cur_id))
            else:
                continue
            
            self.total_pkts.append(Packet(pkt_name, self.cur_id))
            self.cur_id += 1
        
        proto_file.close()

class Packet():
    def __init__(self, name, id):
        self.name = name
        self.id = id