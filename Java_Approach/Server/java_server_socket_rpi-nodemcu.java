import java.io.IOException;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Scanner;

public class ServerEcho {

    public static void main(String[] args) throws UnknownHostException, IOException {

        ServerSocket ss; 
        Socket x;
        PrintWriter output;
        Scanner input;
        String msg1;

        ss = new ServerSocket(3000);
        x = ss.accept();
        System.out.println("Connection ok \n");
        input = new Scanner(x.getInputStream());
        output = new PrintWriter(x.getOutputStream(), true);

        do {

            msg1 = input.nextLine(); 
            System.out.println("Message received \n");
            System.out.println(msg1);
            //System.out.println("Sending back \n");
            //output.println(msg1);

        } while (!msg1.equals("bye"));
        x.close();
        ss.close();
    }
}