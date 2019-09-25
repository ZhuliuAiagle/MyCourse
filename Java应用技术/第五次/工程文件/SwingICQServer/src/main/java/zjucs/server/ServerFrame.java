/*Chelin Tutorials 2012. All Rights Reserved.
 *Educational Purpose Only.Non-Comercial & Profitable.
 *Contact: chelo_c@live.com /chelinho1397@gmail.com
 */

package zjucs.server;
import java.awt.BorderLayout;
import java.awt.Color;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Iterator;
import java.util.Vector;

import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import java.net.InetAddress;
import java.net.UnknownHostException;


import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;


@SuppressWarnings("serial")
public class ServerFrame extends JFrame{



    public static final int PORT = 9999;
    public static final String ADRESS = "localhost";

    Vector<IndividualThread> clients;
    JPanel panel;
    JTextArea textArea;
    ServerSocket serversocket;


    ServerFrame(){
        init_UI();
        start_server();
        listenSocket();
    }


    private void init_UI() {
        setTitle("服务端后台");
        setBounds(200, 200, 400, 400);
        setDefaultCloseOperation(EXIT_ON_CLOSE );


        textArea = new JTextArea();
        textArea.setBackground(Color.white);

        JScrollPane scrollpane = new JScrollPane(textArea);



        panel = new JPanel();

        panel.setLayout(new BorderLayout());
        getContentPane().add(panel);
        panel.add("North", new JLabel("Log:"));
        panel.add("Center", scrollpane);

        setVisible(true);
    }



    private void start_server() {
        try{
            // 获取localhost对应的ip地址，并依据这一ip地址初始化socket server
            InetAddress ip = InetAddress.getByName(ADRESS);
            // 新建socket server
            serversocket = new ServerSocket(PORT,0,ip);
            // 保存所有连接到服务器客户端线程的容器
            clients = new Vector<IndividualThread>();
        }

        catch (UnknownHostException e) {

        }
        catch (IOException e) {
            // 无法将socket服务部署到对应地址/端口后产生的异常
            System.out.println("Could not start conection on port "+PORT);
            System.exit(-1);
        }
        System.out.println("Server started on IP: "+serversocket.getInetAddress());
    }


    public void listenSocket(){

        try{
            while(true){
                Thread.sleep(200);
                // 启动监听线程
                Socket new_conecction= serversocket.accept();

                IndividualThread w = new IndividualThread(new_conecction,this);
                Thread t = new Thread(w);
                t.start();
            }
        }


        catch (IOException e) {
            System.out.println("Accept failed: 4000");
            System.exit(-1);
        }
        catch (InterruptedException e) {
            e.printStackTrace();
        }

    }


    protected void finalize(){
        try{
            System.out.println("Closing server...");
            serversocket.close();
        }
        catch (IOException e) {
            System.out.println("Could not close socket");
            System.exit(-1);
        }
    }


    public void addline(String line) {
        textArea.append(line+"\n");
    }



    public void registerClient(IndividualThread clientthread) {
        clients.add(clientthread);
    }


    public void removeClient(IndividualThread clientthread) {
        clients.remove(clientthread);

    }

    /**向所有客户端发送更新信息**/
    public void broadcast(String parseline) {

        String sendline = parseline;

        Iterator<IndividualThread> it;
        it = clients.iterator();
        int i = 0;
        while(it.hasNext()){
            IndividualThread temp_client = it.next();
            temp_client.send(sendline);

        }
    }

    public static void main(String[] args){
        @SuppressWarnings("unused")
        ServerFrame window = new ServerFrame();
    }
}


class IndividualThread implements Runnable {

    private Socket client;
    private ServerFrame serverFrame;
    private boolean running;

    BufferedReader in = null;
    PrintWriter out = null;

    public IndividualThread(Socket new_conecction, ServerFrame serverFrame) {
        client=new_conecction;
        running=true;
        this.serverFrame=serverFrame;
        this.serverFrame.registerClient(this);

    }


    public void run(){
        create_buffers();

        String line="<!>New Conecction<!> : "+client.getInetAddress();
        System.out.println(line);
        serverFrame.addline(line);
        serverFrame.broadcast(line );

        recieve();

    }



    private void create_buffers() {
        try{
            in = new BufferedReader(new InputStreamReader(client.getInputStream()));
            out = new PrintWriter(client.getOutputStream(), true);
        }
        catch (IOException e) {
            System.out.println("in or out failed");
            System.exit(-1);
        }
    }


    public void recieve(){
        String line,parseline;
        while(running){
            try{
                Thread.sleep(100);
                line = in.readLine();
                if(line!= null){
                    if(check_logout(line))logout(); //如果收到的是logout信息，则登出
                    else{
                        parseline=client.getInetAddress()+"  Said: "+line;
                        parseline=parseline.substring(1);
                        System.out.println(parseline);
                        serverFrame.addline(parseline);
                        serverFrame.broadcast(parseline);
                    }
                }
            }
            //CATCHS
            catch (IOException e) {
                System.out.println("IOERROR INDV THREAD.");
                e.printStackTrace();
                System.exit(-1);
            }
            catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }


    public  void send(String line) {
        out.println(line);
    }




    private boolean check_logout(String line) {
        if (line.length()<7)return false;
        return((line.substring(0, 7).compareTo("/logout")==0));
    }



    private void logout() {
        String line = client.getInetAddress()+" <!>Disconected<!>";
        serverFrame.addline(line);
        serverFrame.broadcast(line);
        running=false;
        serverFrame.removeClient(this);
        try {
            client.close();
        }
        catch (IOException e) {
            e.printStackTrace();
        }

    }



}
