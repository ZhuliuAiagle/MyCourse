package zjucs.client;

import javax.swing.*;
import java.awt.event.*;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Container;
import java.awt.FlowLayout;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.event.ChangeListener;

// 获取客户端服务
import zjucs.client.*;



public class ClientMainUI extends JFrame{

    // 保存有关服务器连接的信息，这里使用的是本地的服务器
    public static final int PORT = 9999;
    public static final String serverAddr = "localhost";
    // 多线程实现
    // 客户端与服务端进行交流使用的socket
    public static Socket socket = null;
    // 出境缓冲区，用于暂时保存需要发送给服务器的消息
    public static PrintWriter serverout = null;
    // 入境缓冲区，用于暂时保存需要从那个服务器读取的消息
    public static BufferedReader serverin = null;
    // 从服务器循环接收消息使用的子线程
    public static Thread  rcvthread;
    // 当前客户端是否处于正常运行状态
    public static Boolean running;


    //GUI组件 -- 通过ClientMainUI生成
    public JTabbedPane tabbedPane1;
    public JPanel panel1;
    public JEditorPane editorPane1;
    public JButton clearButton;
    public JButton 发送Button;
    public JList list2;
    public JTextArea textArea1;

    // 线程子类

    class GetInfoThread extends Thread{
        @Override
        public void run() {
            System.out.println("test");
            while(running)
            {
                // 每次轮询都检测socket连接是否还处于活跃状态
                System.out.println("socket is closed: " + socket.isClosed());
                // 如果socket连接关闭了
                if (socket.isClosed()) running = false;
                System.out.println("while the client is running: " + running);
                // 否则，尝试从入境缓冲区中读取数据
                rcv_data();
                try {
                    // 每次尝试间隔200毫秒
                    Thread.sleep(200);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public void startPolling(){
        rcvthread = new GetInfoThread();
        rcvthread.start();
    }


    public ClientMainUI() {
        //Create a thread that recieves messages
        // 按钮监听器
        发送Button.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                super.mouseClicked(e);
                String t = editorPane1.getText();
                send_data(t);
                textArea1.selectAll();
                editorPane1.setText("");
            }
        });
        this.addWindowListener(new WindowListener() {
            @Override
            public void windowOpened(WindowEvent e) {
                running=true;
                rcvthread = new GetInfoThread();
            }

            @Override
            public void windowClosing(WindowEvent e) {
            }

            @Override
            public void windowClosed(WindowEvent e) {
                running=false;
                logout();
            }

            @Override
            public void windowIconified(WindowEvent e) {

            }

            @Override
            public void windowDeiconified(WindowEvent e) {

            }

            @Override
            public void windowActivated(WindowEvent e) {

            }

            @Override
            public void windowDeactivated(WindowEvent e) {

            }
        });
    }

    // connect方法
    // 在程序启动的时候连接到服务器
    static void srv_connect(){
        try{
            // 初始化socket连接，入境和出境缓冲区
            socket = new Socket(serverAddr,PORT);
            serverout = new PrintWriter(socket.getOutputStream(), true);
            System.out.println(serverout);
            serverin = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            System.out.println(serverin);
        }
        catch (UnknownHostException e) {
            // 异常情况1：找不到主机
            System.out.println("404: Host Not Found");
            System.exit(1);
        }
        catch  (IOException e) {
            // 异常情况2：找到了主机但主机拒绝连接
            System.out.println("403: Forbidden (maybe server isn't active)");
            System.exit(1);
        }
        running = true;
    }

    //InitUI方法
    // 在程序启动的时候初始化用户图形界面
    static void InitUI(){
        JFrame frame = new JFrame("SwingICQ");
        ClientMainUI c = new ClientMainUI();
        frame.setContentPane(c.panel1);
        //frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.pack();
        c.startPolling();
        frame.setVisible(true);
    }

    // send_data方法
    // 向往服务器发送消息的缓冲区中输入数据
    private void send_data(String text){
        try {
            System.out.println(text);
            serverout.println(text);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }
    // rcv_data方法
    // 从服务器socket连接中读取数据
    private void rcv_data(){
        String line;
        try{
            // 尝试从入境缓冲区中读取字符串
            line = serverin.readLine();
            if(line!=null){
                // 如果读取到字符串
                System.out.println("rcv:"+line);
                // 将字符串写入UI界面的textArea
                textArea1.append(line+"\n");
                textArea1.selectAll();
            }
        }
        // 异常处理
        catch (IOException e){
            System.out.println("Can't receive data, maybe you should check the Internet!");
            e.printStackTrace();
            System.exit(1);
        }
    }
    //  run 方法
    // 启动socket服务,循环从服务器接收数据
    // 从服务器端登出
    public void logout() {
        send_data("/logout");
        running=false;
        try {
            Thread.sleep(5000);
            socket.close();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        // 连接服务器
        srv_connect();
        InitUI();

    }
}


