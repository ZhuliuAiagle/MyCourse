import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        System.out.println("Please select mode:\n1.DecToHex\n2.HexToDec");
        Scanner sc = new Scanner(System.in);
        int a = sc.nextInt();
        if(a == 1){
            System.out.println("Please enter the num:");
            Scanner sc2 = new Scanner(System.in);
            int b = sc2.nextInt();
            System.out.println(DecToHex(b));
        }else if(a == 2){
            System.out.println("Please enter the num:");
            Scanner sc3 = new Scanner(System.in);
            String b = sc3.nextLine();
            System.out.println(HexToDec(b));
        }

    }
    public static String DecToHex(int a){
        Integer x = a;
        return Integer.toHexString(x);
    }
    public static int HexToDec(String hex){
        return Integer.parseInt(hex.substring(2),16);
    }
}
