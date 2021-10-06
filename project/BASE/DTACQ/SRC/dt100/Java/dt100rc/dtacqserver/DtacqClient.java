/*****************************************************************************
 *
 * File:
 *
 * $RCSfile: DtacqClient.java,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: DtacqClient.java,v 1.1.4.4 2004/09/21 12:30:50 pgm Exp $
 * $Log: DtacqClient.java,v $
 * Revision 1.1.4.4  2004/09/21 12:30:50  pgm
 * B5221
 * Supports host:port notation - useful for ssh tunnels to localhost
 *
 * Revision 1.1.4.3  2004/08/14 20:44:42  pgm
 * B5216
 * fix crazy "Board as Singleton" bug
 *
 * Revision 1.1.4.2  2004/06/26 22:57:24  pgm
 * Build 5210 MF stats look good.
 * Teed up for previous second control output.
 * (see DtacqClient.connectRaw()).
 *
 * Revision 1.1.4.1  2004/06/26 19:59:09  pgm
 * serious grief failed to serialize stream Combos. fake it by introducing defaults MF Stats in but non-func
 *
 * Revision 1.1  2002/05/17 08:25:08  pgm
 * subset needed for dt100rc
 *
 * Revision 1.3  2000/04/18 09:45:40  pgm
 * repaired channel comms, primitive status rep
 *
 * Revision 1.2  2000/04/15 13:45:31  pgm
 * added headers
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
 \*****************************************************************************/


package dtacqserver;

// DtacqClient.java

import debug.Debug;

import java.net.*;
import java.io.*;
import java.util.*;
import java.text.*;

class DtacqClientImpl extends DtacqClient implements Serializable {
    private Socket socket;
    private DataInputStream in;
    private PrintWriter out;

    public PrintWriter getOutput() {
        return out;
    }

    public DataInputStream getInput() {
        return in;
    }

    public boolean isAvailable() {
        try {
            return socket.getInputStream() != null;
        }catch(IOException e){
            return false;
        }
    }

    public DtacqClientImpl(InetAddress addr,
                           String hostname,
                           String stream_id,
                           int port, boolean expect_signon) throws IOException {

        super(hostname, stream_id);

        try {
            socket = new Socket();
            if (request_max_rxbuffer) {
                socket.setReceiveBufferSize(0x100000);
            }
            socket.connect(new InetSocketAddress(addr, port));
            Debug.println("Socket RCV_BUFFER_SIZE" + socket.getReceiveBufferSize());
        } catch (IOException e) {
            System.out.println("ERROR:failed to create socket " + e);
            throw e;
        }
        try {
            in = new DataInputStream(new BufferedInputStream(socket.getInputStream()));
            // Enable auto-flush:
            out = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())), true);

            if (expect_signon) {
                String sign_on = in.readLine();
            }
            //System.out.println( "Signing on:" + sign_on );
        } catch (IOException e) {
            System.out.println("ERROR: failed to open streams " + e);


            try {
                cleanup();
            } catch (IOException e2) {
                System.out.println("ERROR: failed to close streams " + e2);
            }
            throw e;                // throw first error
        }
    }

    public void cleanup() throws IOException {
        socket.close();
    }
    // Otherwise the socket will be closed by
    // the run() method of the thread.
}

public abstract class DtacqClient extends ReadWriteStream {
    // Factory class: can't be instantiated directly, only thru connect()
  
    static Map connection_map = new HashMap();

    final String hostname;
    final String stream_id;

    static boolean request_max_rxbuffer;

    public static void setRequest_max_rxbuffer(boolean req) {
        request_max_rxbuffer = req;
    }

    static String makeKey(String hostname, String stream_id) {
        return new String(hostname + ":" + stream_id);
    }

    String getKey() {
        return makeKey(hostname, stream_id);
    }

    protected DtacqClient(String hostname, String stream_id) {
        this.hostname = hostname;
        this.stream_id = stream_id;
    }

    public static ReadWriteStream connect(String hostname, String stream_id)
            throws IOException {

        String key = makeKey(hostname, stream_id);

        String hp [] = hostname.split(":");

        String host = hp[0];
        int port = hp.length == 2 ? Integer.parseInt(hp[1]) : DtacqComms.PORT;

        Debug.print(1, "hostname:" + hostname + " host:" + host + " port:" + port);

        DtacqClientImpl the_client;

        if (connection_map.containsKey(key)) {

            the_client = (DtacqClientImpl) connection_map.get(key);

            if (the_client.isAvailable()) {
                return the_client;
            } else {
                connection_map.remove(key);
            }
        }

        InetAddress addr;

        try {
            addr = InetAddress.getByName(host);
        } catch (UnknownHostException e) {
            System.out.println("ERROR: Host:" + host + " " + e);
            throw e;
        }
        the_client = new DtacqClientImpl(addr, host, stream_id, port, true);
        connection_map.put(key, the_client);
        return the_client;
}

    public static ReadWriteStream connectRaw(String hostname, int port)
            throws IOException {
        InetAddress addr;

        try {
            addr = InetAddress.getByName(hostname);
        } catch (UnknownHostException e) {
            System.out.println("ERROR: Host:" + hostname + " " + e);
            throw e;
        }
        DtacqClientImpl the_client = new DtacqClientImpl(addr, "raw", hostname, port, false);

        return the_client;
    }

    public static void close(ReadWriteStream client) {
        DtacqClient dtacqClient = (DtacqClient) client;
        connection_map.remove(dtacqClient.getKey());
        client.close();
    }

    public static void main(String args[])
            throws IOException, InterruptedException {


        String hostname = args.length >= 1 ? args[0] : "harris";
        String stream_id = args.length >= 2 ? args[1] : "acq32.1.m1";

        ReadWriteStream rws = connect(hostname, stream_id);

        BufferedReader in =
                new BufferedReader(new InputStreamReader(System.in));

        while (true) {
            String command = new String();
            String response = new String();

            command = in.readLine();

            if (command == null) {
                break;
            } else if (command.startsWith("#")) {
                System.out.println(command);
            } else {
                rws.getOutput().println(command);
                response = rws.getInput().readLine();
                System.out.println(response);

                if (command.equals("bye")) {
                    break;
                }
            }
        }
    }
}
