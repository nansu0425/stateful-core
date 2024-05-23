
import argparse
import jinja2
import ProtoParser

def main():
    arg_parser = argparse.ArgumentParser(description='PacketHandlerGenerator')
    arg_parser.add_argument('--path', type=str, default='Packet.proto', help='proto path')
    arg_parser.add_argument('--output', type=str, default='PacketHandler', help='output file name')
    arg_parser.add_argument('--recv', type=str, default='C_', help='recv packet prefix')
    arg_parser.add_argument('--send', type=str, default='S_', help='send packet prefix')
    
    args = arg_parser.parse_args()
    proto_parser = ProtoParser.ProtoParser(1000, args.recv, args.send)
    proto_parser.parse_proto(args.path)

    file_loader = jinja2.FileSystemLoader('templates')
    env = jinja2.Environment(loader=file_loader)
    
    template = env.get_template('PacketHandler.hpp')
    rendered_code = template.render(parser=proto_parser, output=args.output)
    
    f = open(args.output + '.hpp', 'w+')
    f.write(rendered_code)
    f.close()

    return

if __name__ == '__main__':
    main()
