struct tcp_hdr{
                short int src;
                short int des;
                int seq;
                int ack;
                short int hdr_flags;
                short int rec;
                short int cksum;
                short int ptr;
                int opt;
};

