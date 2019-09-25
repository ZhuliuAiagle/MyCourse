import java.util.*;
import java.lang.*;


enum Operation {
    PLUS, MINUS,EQUAL
}

public class ResetMatch {
    public static void main(String[] args){
        System.out.println("欢迎来到火柴棒游戏：Powered by 沈子衿");
        System.out.println("请输入最大数字的位数:");
        Scanner sc = new Scanner(System.in);
        int maxDigit = sc.nextInt();
        System.out.println("请输入等号左式数字个数:");
        sc = new Scanner(System.in);
        int leftNum = sc.nextInt();
        System.out.println("请输入游戏模式:");
        System.out.println("1.移动 2.添加 3.移除");
        sc = new Scanner(System.in);
        int mode = sc.nextInt();
        System.out.println("请输入要移动火柴棒根数，本游戏只支持1-2根火柴棒:");
        sc = new Scanner(System.in);
        int matchNum = sc.nextInt();
        System.out.println("正在为您生成题目...生成成功！");
        Equation eq = new Equation(maxDigit, leftNum);
        /* eq.ExhibitInAscii(); //这是正确答案 */
        eq.move(mode,matchNum);
        /* for(int i = 0; i < eq.movResult.size(); i++){
            EquationMap eqm = new EquationMap(eq.movResult.get(i));
            eq.movResult.get(i).ExhibitInAscii();
        } */
        EquationMap eqm;
        while(true){
            int num = (int)(0+Math.random()*((eq.movResult.size()-1)-1+1));
            eqm = new EquationMap(eq.movResult.get(num));
            if((int)Math.abs(eq.movResult.get(num).NumOfMatch()-eq.NumOfMatch()) > 2) continue;
            break;
        } 
        eqm.Exhibit();
        System.out.print("请输入你的答案，直接敲击Enter键查看标准答案:");
        // 开始解析输入
        while(true){
            sc = new Scanner(System.in);
            String input = sc.nextLine();
            input.replace(" ","");
            input.replace("\t","");
            if(input.length() == 0) {
                System.out.println("输出正确答案!");
                break;
            }
            String[] sp = input.split("[\\+\\-\\=]");
            //开始判断
            if(sp.length != eq.numberList.length + 1){
                System.out.println("答案错误!");
                continue;
            }
            int i;
            for(i = 0; i < sp.length - 1; i++){
                if(Integer.parseInt(sp[i]) != eq.numberList[i]){
                    break;
                }
            }
            if(i < sp.length - 1){
                System.out.println("答案错误!");
                continue;
            }
            if(Integer.parseInt(sp[sp.length - 1]) != eq.result){
                System.out.println("答案错误!");
                continue;
            }
            System.out.println("答案正确!");
            break;
        }
        eq.ExhibitInAscii();
    }
}
class Equation {
    int[] numberList;
    Operation[] opList; // Oplist must have 1 less item than numberList
    int result;
    ArrayList<Equation> movResult;
    //构造函数一：随机生成一个正确的等式
    Equation(int maxDigit, int leftNum){
        this.result = 0;
        this.numberList = new int[leftNum];
        this.opList = new Operation[leftNum - 1];
        while(true){
            for(int i = 0; i < leftNum - 1; i++){
                int a = (int)(1+Math.random()*((int)(3-1+1)));
                if(a == 1)
                    opList[i] = Operation.PLUS;
                else if (a == 2)
                    opList[i] = Operation.PLUS;
                else{
                    opList[i] = Operation.MINUS;
                }
            }
            numberList[0] = (int)(1+Math.random()*((int)(Math.pow(10,maxDigit))-1+1-1));
            result = numberList[0];
            for(int i = 1; i < leftNum; i++){
                numberList[i] = (int)(1+Math.random()*((int)(Math.pow(10,maxDigit))-1+1-1));
                if(opList[i-1] == Operation.PLUS)
                    this.result += numberList[i];
                else{
                    this.result -= numberList[i];
                }
            }
            if(result < 0){
                result = 0;
            }
            else if((int)(result / Math.pow(10,maxDigit)) > 0){
                result = 0;
            }
            else{
                break;
            }
        }
        movResult = new ArrayList<Equation>();
    }
    //构造函数二：按照列表生成一个等式
    Equation(int[] nlist, Operation[] olist, int result){
        this.result = result;
        this.numberList = new int[nlist.length];
        this.opList = new Operation[olist.length];
        for(int i = 0; i < this.numberList.length; i++) this.numberList[i] = nlist[i];
        for(int i = 0; i < this.opList.length; i++)this.opList[i] = olist[i];
        movResult = new ArrayList<Equation>();
    }
    //构造函授三：用现有实例复制一个新等式
    Equation(Equation a){
        this.result = a.result;
        this.numberList = new int[a.numberList.length];
        this.opList = new Operation[a.opList.length];
        for(int i = 0; i < this.numberList.length; i++) this.numberList[i] = a.numberList[i];
        for(int i = 0; i < this.opList.length; i++)this.opList[i] = a.opList[i];
        movResult = new ArrayList<Equation>();
    }
    //以字符方式展示
    void ExhibitInAscii(){
        if(this.IsEmpty() || (!this.IsValid())) {System.out.print("invalid!");return;}
        System.out.print(this.numberList[0]);
        for(int i = 1; i < numberList.length; i++){
            if(this.opList[i-1] == Operation.PLUS) System.out.print(" + ");
            else System.out.print(" - ");
            System.out.print(this.numberList[i]);
        }
        System.out.print(" = ");
        System.out.print(result);
        System.out.print("\n");

    }
    //求火柴总数
    int NumOfMatch(){
        int sum = 0;
        for(int i = 0; i < numberList.length; i++){
            if(numberList[i] == 0) sum += 6;
            else if(numberList[i] == 1) sum += 2;
            else if(numberList[i] == 2) sum += 5;
            else if(numberList[i] == 3) sum += 5;
            else if(numberList[i] == 4) sum += 4;
            else if(numberList[i] == 5) sum += 5;
            else if(numberList[i] == 6) sum += 6;
            else if(numberList[i] == 7) sum += 3;
            else if(numberList[i] == 8) sum += 7;
            else if(numberList[i] == 9) sum += 6;
        }
        for(int i = 0; i < opList.length; i++){
            if(opList[i] == Operation.PLUS) sum += 2;
            else if(opList[i] == Operation.MINUS) sum += 1;
        }
        sum += 2;
        return sum;
    }
    //判断等式是否为空
    Boolean IsEmpty(){
        if(numberList.length == 0){
            return true;
        }
        return false;
    }
    // 判断等式是否有效
    Boolean IsValid(){
        if(this.IsEmpty()) return false;
        if(numberList.length > 1 && opList.length > 0 && 
        numberList.length == opList.length + 1) return true;
        return false;
    }
    // 判断等式是否成立
    Boolean IsEqual(){
        if(this.IsEmpty()) return false;
        int ActualResult = numberList[0]; // 把第一个作为初始值，然后不断加减
        for(int i = 0; i < opList.length; i++){
            Operation temp = opList[i];
            if(temp == Operation.PLUS){
                ActualResult += numberList[i+1];
            }else{
                ActualResult -= numberList[i+1];
            }
        }
        if(ActualResult == this.result) return true;
        else return false;
    }
    //出题的核心代码
    void move(int mode, int matchNum){
        Graph g = new Graph();
        g.BuildGraph();
        int[] bknl = new int[this.numberList.length];
        for(int i = 0; i < this.numberList.length; i++) bknl[i] = this.numberList[i];
        Operation[] bkop = new Operation[this.opList.length];
        for(int i = 0; i < this.opList.length; i++) bkop[i] = this.opList[i];
        int bkrs = result;
        //开始分类讨论
        //移动模式
        if(mode == 1){
            // 如果只移动一根火柴棒
            if(matchNum == 1){
                //第一种情况，在数字间寻找+1和-1
                for(int i = 0; i < numberList.length + 1; i++){
                    Numeric[] numList = new Numeric[numberList.length + 1];
                    for(int s = 0; s < numberList.length; s++){
                        // 如果大于10需要额外解析
                        numList[s] = new Numeric(numberList[s]);
                    }
                    numList[numberList.length] = new Numeric(result);
                    Numeric tp = numList[i];
                    Numeric tp2 = new Numeric(numList[i].number);
                    for(int j = 0; j < tp.digitList.size(); j++){
                        for(int m = 0; m < tp.digitList.size(); m++){
                            tp.digitList.set(m,tp2.digitList.get(m));
                        }
                        tp.RenewValue();
                        int temp = (int)tp.digitList.get(j);
                        if(g.graphArray[temp].next.length != 0){
                            for(int k = 0; k < g.graphArray[temp].next.length; k++){
                                // 如果+1成了
                                if(g.graphArray[temp].next[k] == 1){
                                    for(int d = 0; d < tp.digitList.size() && d != j; d++){
                                        for(int m = 0; m < tp.digitList.size(); m++){
                                            tp.digitList.set(m,tp2.digitList.get(m));
                                        }
                                        tp.RenewValue();
                                        int temp2 = (int)tp.digitList.get(d);
                                        for(int b = 0; b < g.graphArray[temp2].previous.length; b++){
                                            if(g.graphArray[temp2].previous[b] == 1){
                                                numList[i].digitList.set(j, k);
                                                numList[i].digitList.set(d, b);
                                                numList[i].RenewValue();
                                                for(int p = 0; p < numberList.length; p++){
                                                    numberList[p] = numList[p].number;
                                                }
                                                result = numList[numberList.length].number;
                                                Equation a = new Equation(this.numberList,this.opList,this.result);
                                                this.movResult.add(a);
                                                //还原this
                                                for(int p = 0; p < this.numberList.length; p++) this.numberList[p] = bknl[p];
                                                for(int p = 0; p < this.opList.length; p++) this.opList[p] = bkop[p];
                                                this.result = bkrs;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                // 第二种情况，考虑运算符之间的移动，一种是加号移到数字上
                for(int i = 0; i < numberList.length + 1; i++){
                    Numeric[] numList = new Numeric[numberList.length + 1];
                    for(int s = 0; s < numberList.length; s++){
                        // 如果大于10需要额外解析
                        numList[s] = new Numeric(numberList[s]);
                    }
                    numList[numberList.length] = new Numeric(result);
                    Numeric tp = numList[i];
                    Numeric tp2 = new Numeric(numList[i].number);
                    for(int j = 0; j < tp.digitList.size(); j++){
                        for(int m = 0; m < tp.digitList.size(); m++){
                            tp.digitList.set(m,tp2.digitList.get(m));
                        }
                        tp.RenewValue();
                        int temp = (int)tp.digitList.get(j);
                        if(g.graphArray[temp].next.length != 0){
                            for(int k = 0; k < g.graphArray[temp].next.length; k++){
                                if(g.graphArray[temp].next[k] == 1){
                                    for(int d = 0; d < opList.length; d++)
                                        if(opList[d] == Operation.PLUS){
                                            opList[d] = Operation.MINUS;
                                            numList[i].digitList.set(j, k);
                                            numList[i].RenewValue();
                                            for(int p = 0; p < numberList.length; p++){
                                                numberList[p] = numList[p].number;
                                            }
                                            result = numList[numberList.length].number;
                                            Equation a = new Equation(this.numberList,this.opList,this.result);
                                            this.movResult.add(a);
                                            //还原this
                                            for(int p = 0; p < this.numberList.length; p++) this.numberList[p] = bknl[p];
                                            for(int p = 0; p < this.opList.length; p++) this.opList[p] = bkop[p];
                                            this.result = bkrs;
                                        }     
                                }
                            }
                        }
                    }
                }
                // 第三种情况，数字移动到减号上
                for(int i = 0; i < numberList.length + 1; i++){
                    Numeric[] numList = new Numeric[numberList.length + 1];
                    for(int s = 0; s < numberList.length; s++){
                        // 如果大于10需要额外解析
                        numList[s] = new Numeric(numberList[s]);
                    }
                    numList[numberList.length] = new Numeric(result);
                    Numeric tp = numList[i];
                    Numeric tp2 = new Numeric(numList[i].number);
                    for(int j = 0; j < tp.digitList.size(); j++){
                        for(int m = 0; m < tp.digitList.size(); m++){
                            tp.digitList.set(m,tp2.digitList.get(m));
                        }
                        tp.RenewValue();
                        int temp = (int)tp.digitList.get(j);
                        if(g.graphArray[temp].previous.length != 0){
                            for(int k = 0; k < g.graphArray[temp].previous.length; k++){
                                if(g.graphArray[temp].previous[k] == 1){
                                    for(int d = 0; d < opList.length; d++)
                                        if(opList[d] == Operation.MINUS){
                                            opList[d] = Operation.PLUS;
                                            numList[i].digitList.set(j, k);
                                            numList[i].RenewValue();
                                            for(int p = 0; p < numberList.length; p++){
                                                numberList[p] = numList[p].number;
                                            }
                                            this.result = numList[numberList.length].number;
                                            //存入列表
                                            Equation a = new Equation(this.numberList,this.opList,this.result);
                                            this.movResult.add(a);
                                            //还原this
                                            for(int p = 0; p < this.numberList.length; p++) this.numberList[p] = bknl[p];
                                            for(int p = 0; p < this.opList.length; p++) this.opList[p] = bkop[p];
                                            this.result = bkrs;
                                        }
                                }
                            }
                        }
                    }
                }
                //第三种情况自环
                for(int i = 0; i < numberList.length + 1; i++){
                    Numeric[] numList = new Numeric[numberList.length + 1];
                    for(int s = 0; s < numberList.length; s++){
                        // 如果大于10需要额外解析
                        numList[s] = new Numeric(numberList[s]);
                    }
                    numList[numberList.length] = new Numeric(result);
                    Numeric tp = numList[i];
                    Numeric tp2 = new Numeric(numList[i].number);
                    for(int j = 0; j < tp.digitList.size(); j++){
                        for(int m = 0; m < tp.digitList.size(); m++){
                            tp.digitList.set(m,tp2.digitList.get(m));
                        }
                        tp.RenewValue();
                        int temp = (int)tp.digitList.get(j);
                        if(g.graphArray[temp].previous.length != 0){
                            for(int k = 0; k < g.graphArray[temp].previous.length; k++){
                                if(g.graphArray[temp].next[k] == 3){
                                    numList[i].digitList.set(j, k);
                                    numList[i].RenewValue();
                                    for(int p = 0; p < numberList.length; p++){
                                        numberList[p] = numList[p].number;
                                    }
                                    this.result = numList[numberList.length].number;
                                    //存入列表
                                    Equation a = new Equation(this.numberList,this.opList,this.result);
                                    this.movResult.add(a);
                                    //还原this
                                    for(int p = 0; p < this.numberList.length; p++) this.numberList[p] = bknl[p];
                                    for(int p = 0; p < this.opList.length; p++) this.opList[p] = bkop[p];
                                    this.result = bkrs;
                                }
                            }
                        }
                    }
                }
            }
            else if(matchNum == 2){
                //第一种情况，在数字间寻找+2和-2
                for(int i = 0; i < numberList.length + 1; i++){
                    Numeric[] numList = new Numeric[numberList.length + 1];
                    for(int s = 0; s < numberList.length; s++){
                        // 如果大于10需要额外解析
                        numList[s] = new Numeric(numberList[s]);
                    }
                    numList[numberList.length] = new Numeric(result);
                    Numeric tp = numList[i];
                    Numeric tp2 = new Numeric(numList[i].number);
                    for(int j = 0; j < tp.digitList.size(); j++){
                        for(int m = 0; m < tp.digitList.size(); m++){
                            tp.digitList.set(m,tp2.digitList.get(m));
                        }
                        tp.RenewValue();
                        int temp = (int)tp.digitList.get(j);
                        if(g.graphArray[temp].next.length != 0){
                            for(int k = 0; k < g.graphArray[temp].next.length; k++){
                                // 如果+1成了
                                if(g.graphArray[temp].next[k] == 2){
                                    for(int d = 0; d < tp.digitList.size() && d != j; d++){
                                        for(int m = 0; m < tp.digitList.size(); m++){
                                            tp.digitList.set(m,tp2.digitList.get(m));
                                        }
                                        tp.RenewValue();
                                        int temp2 = (int)tp.digitList.get(d);
                                        for(int b = 0; b < g.graphArray[temp2].previous.length; b++){
                                            if(g.graphArray[temp2].previous[b] == 2){
                                                numList[i].digitList.set(j, k);
                                                numList[i].digitList.set(d, b);
                                                numList[i].RenewValue();
                                                for(int p = 0; p < numberList.length; p++){
                                                    numberList[p] = numList[p].number;
                                                }
                                                result = numList[numberList.length].number;
                                                Equation a = new Equation(this.numberList,this.opList,this.result);
                                                this.movResult.add(a);
                                                //还原this
                                                for(int p = 0; p < this.numberList.length; p++) this.numberList[p] = bknl[p];
                                                for(int p = 0; p < this.opList.length; p++) this.opList[p] = bkop[p];
                                                this.result = bkrs;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                // 第二种情况，在数字间寻找+2和-1 -1
                for(int i = 0; i < numberList.length + 1; i++){
                    Numeric[] numList = new Numeric[numberList.length + 1];
                    for(int s = 0; s < numberList.length; s++){
                        // 如果大于10需要额外解析
                        numList[s] = new Numeric(numberList[s]);
                    }
                    numList[numberList.length] = new Numeric(result);
                    Numeric tp = numList[i];
                    Numeric tp2 = new Numeric(numList[i].number);
                    for(int j = 0; j < tp.digitList.size(); j++){
                        for(int m = 0; m < tp.digitList.size(); m++){
                            tp.digitList.set(m,tp2.digitList.get(m));
                        }
                        tp.RenewValue();
                        int temp = (int)tp.digitList.get(j);
                        if(g.graphArray[temp].next.length != 0){
                            for(int k = 0; k < g.graphArray[temp].next.length; k++){
                                // 如果+1成了
                                if(g.graphArray[temp].next[k] == 2){
                                    for(int d = 0; d < tp.digitList.size() && d != j; d++){
                                        for(int m = 0; m < tp.digitList.size(); m++){
                                            tp.digitList.set(m,tp2.digitList.get(m));
                                        }
                                        tp.RenewValue();
                                        int temp2 = (int)tp.digitList.get(d);
                                        for(int b = 0; b < g.graphArray[temp2].previous.length; b++){
                                            if(g.graphArray[temp2].previous[b] == 1){
                                                for(int x = 0; x < tp.digitList.size() && x != j && x != d; x++){
                                                    for(int m = 0; m < tp.digitList.size(); m++){
                                                        tp.digitList.set(m,tp2.digitList.get(m));
                                                    }
                                                    tp.RenewValue();
                                                    int temp3 = (int)tp.digitList.get(x);
                                                    for(int y = 0; y < g.graphArray[temp3].previous.length; y++){
                                                        if(g.graphArray[temp3].previous[y] == 1){
                                                            numList[i].digitList.set(j, k);
                                                            numList[i].digitList.set(d, b);
                                                            numList[i].digitList.set(x, y);
                                                            numList[i].RenewValue();
                                                            for(int p = 0; p < numberList.length; p++){
                                                                numberList[p] = numList[p].number;
                                                            }
                                                            result = numList[numberList.length].number;
                                                            Equation a = new Equation(this.numberList,this.opList,this.result);
                                                            this.movResult.add(a);
                                                            //还原this
                                                            for(int p = 0; p < this.numberList.length; p++) this.numberList[p] = bknl[p];
                                                            for(int p = 0; p < this.opList.length; p++) this.opList[p] = bkop[p];
                                                            this.result = bkrs;
                                                        }
                                                    }

                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                //第三种情况，在数字中寻找-2和+1+1
                for(int i = 0; i < numberList.length + 1; i++){
                    Numeric[] numList = new Numeric[numberList.length + 1];
                    for(int s = 0; s < numberList.length; s++){
                        // 如果大于10需要额外解析
                        numList[s] = new Numeric(numberList[s]);
                    }
                    numList[numberList.length] = new Numeric(result);
                    Numeric tp = numList[i];
                    Numeric tp2 = new Numeric(numList[i].number);
                    for(int j = 0; j < tp.digitList.size(); j++){
                        for(int m = 0; m < tp.digitList.size(); m++){
                            tp.digitList.set(m,tp2.digitList.get(m));
                        }
                        tp.RenewValue();
                        int temp = (int)tp.digitList.get(j);
                        if(g.graphArray[temp].next.length != 0){
                            for(int k = 0; k < g.graphArray[temp].next.length; k++){
                                // 如果+1成了
                                if(g.graphArray[temp].previous[k] == 2){
                                    for(int d = 0; d < tp.digitList.size() && d != j; d++){
                                        for(int m = 0; m < tp.digitList.size(); m++){
                                            tp.digitList.set(m,tp2.digitList.get(m));
                                        }
                                        tp.RenewValue();
                                        int temp2 = (int)tp.digitList.get(d);
                                        for(int b = 0; b < g.graphArray[temp2].previous.length; b++){
                                            if(g.graphArray[temp2].next[b] == 1){
                                                for(int x = 0; x < tp.digitList.size() && x != j && x != d; x++){
                                                    for(int m = 0; m < tp.digitList.size(); m++){
                                                        tp.digitList.set(m,tp2.digitList.get(m));
                                                    }
                                                    tp.RenewValue();
                                                    int temp3 = (int)tp.digitList.get(x);
                                                    for(int y = 0; y < g.graphArray[temp3].previous.length; y++){
                                                        if(g.graphArray[temp3].next[y] == 1){
                                                            numList[i].digitList.set(j, k);
                                                            numList[i].digitList.set(d, b);
                                                            numList[i].digitList.set(x, y);
                                                            numList[i].RenewValue();
                                                            for(int p = 0; p < numberList.length; p++){
                                                                numberList[p] = numList[p].number;
                                                            }
                                                            result = numList[numberList.length].number;
                                                            Equation a = new Equation(this.numberList,this.opList,this.result);
                                                            this.movResult.add(a);
                                                            //还原this
                                                            for(int p = 0; p < this.numberList.length; p++) this.numberList[p] = bknl[p];
                                                            for(int p = 0; p < this.opList.length; p++) this.opList[p] = bkop[p];
                                                            this.result = bkrs;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                //第四种情况：自环2
                for(int i = 0; i < numberList.length + 1; i++){
                    Numeric[] numList = new Numeric[numberList.length + 1];
                    for(int s = 0; s < numberList.length; s++){
                        // 如果大于10需要额外解析
                        numList[s] = new Numeric(numberList[s]);
                    }
                    numList[numberList.length] = new Numeric(result);
                    Numeric tp = numList[i];
                    Numeric tp2 = new Numeric(numList[i].number);
                    for(int j = 0; j < tp.digitList.size(); j++){
                        for(int m = 0; m < tp.digitList.size(); m++){
                            tp.digitList.set(m,tp2.digitList.get(m));
                        }
                        tp.RenewValue();
                        int temp = (int)tp.digitList.get(j);
                        if(g.graphArray[temp].previous.length != 0){
                            for(int k = 0; k < g.graphArray[temp].previous.length; k++){
                                if(g.graphArray[temp].next[k] == 4){
                                    numList[i].digitList.set(j, k);
                                    numList[i].RenewValue();
                                    for(int p = 0; p < numberList.length; p++){
                                        numberList[p] = numList[p].number;
                                    }
                                    this.result = numList[numberList.length].number;
                                    //存入列表
                                    Equation a = new Equation(this.numberList,this.opList,this.result);
                                    this.movResult.add(a);
                                    //还原this
                                    for(int p = 0; p < this.numberList.length; p++) this.numberList[p] = bknl[p];
                                    for(int p = 0; p < this.opList.length; p++) this.opList[p] = bkop[p];
                                    this.result = bkrs;
                                }
                            }
                        }
                    }
                }    
            }
        }
        //移除
        else if(mode == 2){
            if(matchNum == 1){
                // 首先从第一个数字开始尝试
                for(int i = 0; i < numberList.length + 1; i++){
                    Numeric[] numList = new Numeric[numberList.length + 1];
                    for(int s = 0; s < numberList.length; s++){
                        // 如果大于10需要额外解析
                        numList[s] = new Numeric(numberList[s]);
                    }
                    numList[numberList.length] = new Numeric(result);
                    Numeric tp = numList[i];
                    //做备份
                    Numeric tp2 = new Numeric(numList[i].number);
                    for(int j = 0; j < tp.digitList.size(); j++){
                        for(int m = 0; m < tp.digitList.size(); m++){
                            tp.digitList.set(m,tp2.digitList.get(m));
                        }
                        tp.RenewValue();
                        int temp = (int)tp.digitList.get(j);
                        if(g.graphArray[temp].previous.length != 0){
                            for(int k = 0; k < g.graphArray[temp].previous.length; k++){
                                if(g.graphArray[temp].previous[k] == 1){
                                    numList[i].digitList.set(j, k);
                                    numList[i].RenewValue();
                                    for(int p = 0; p < numberList.length; p++){
                                        numberList[p] = numList[p].number;
                                    }
                                    this.result = numList[numberList.length].number;
                                    //存入列表
                                    Equation a = new Equation(this.numberList,this.opList,this.result);
                                    this.movResult.add(a);
                                    //还原this
                                    for(int p = 0; p < this.numberList.length; p++) this.numberList[p] = bknl[p];
                                    for(int p = 0; p < this.opList.length; p++) this.opList[p] = bkop[p];
                                    this.result = bkrs;
                                }
                            }
                        }
                    }
                }
                //然后尝试减少加减号
                for(int i = 0; i < opList.length; i++){
                    if(opList[i] == Operation.PLUS){
                        opList[i] = Operation.MINUS;
                        Equation a = new Equation(this.numberList,this.opList,this.result);
                        this.movResult.add(a);
                        //还原this
                        for(int p = 0; p < this.numberList.length; p++) this.numberList[p] = bknl[p];
                        for(int p = 0; p < this.opList.length; p++) this.opList[p] = bkop[p];
                        this.result = bkrs;
                    }
                } 
            }
            // 如果移动的火柴数是两根
            // 那基本有两种情况，分别是一个单独2过去，或者逆溯两次
            if(matchNum == 2){
                // 情况1：单独2过去
                for(int i = 0; i < numberList.length + 1; i++){
                    Numeric[] numList = new Numeric[numberList.length + 1];
                    for(int s = 0; s < numberList.length; s++){
                        // 如果大于10需要额外解析
                        numList[s] = new Numeric(numberList[s]);
                    }
                    numList[numberList.length] = new Numeric(result);
                    Numeric tp = numList[i];
                    Numeric tp2 = new Numeric(numList[i].number);
                    for(int j = 0; j < tp.digitList.size(); j++){
                        for(int m = 0; m < tp.digitList.size(); m++){
                            tp.digitList.set(m,tp2.digitList.get(m));
                        }
                        tp.RenewValue();
                        int temp = (int)tp.digitList.get(j);
                        if(g.graphArray[temp].previous.length != 0){
                            for(int k = 0; k < g.graphArray[temp].previous.length; k++){
                                if(g.graphArray[temp].previous[k] == 2){
                                    numList[i].digitList.set(j, k);
                                    numList[i].RenewValue();
                                    for(int p = 0; p < numberList.length; p++){
                                        numberList[p] = numList[p].number;
                                    }
                                    this.result = numList[numberList.length].number;
                                    //存入列表
                                    Equation a = new Equation(this.numberList,this.opList,this.result);
                                    this.movResult.add(a);
                                    //还原this
                                    for(int p = 0; p < this.numberList.length; p++) this.numberList[p] = bknl[p];
                                    for(int p = 0; p < this.opList.length; p++) this.opList[p] = bkop[p];
                                    this.result = bkrs;
                                }
                            }
                        }
                    }
                }
                //情况2：-1-1
                for(int i = 0; i < numberList.length + 1; i++){
                    Numeric[] numList = new Numeric[numberList.length + 1];
                    for(int s = 0; s < numberList.length; s++){
                        // 如果大于10需要额外解析
                        numList[s] = new Numeric(numberList[s]);
                    }
                    numList[numberList.length] = new Numeric(result);
                    Numeric tp = numList[i];
                    Numeric tp2 = new Numeric(numList[i].number);
                    for(int j = 0; j < tp.digitList.size(); j++){
                        for(int m = 0; m < tp.digitList.size(); m++){
                            tp.digitList.set(m,tp2.digitList.get(m));
                        }
                        tp.RenewValue();
                        int temp = (int)tp.digitList.get(j);
                        if(g.graphArray[temp].next.length != 0){
                            for(int k = 0; k < g.graphArray[temp].next.length; k++){
                                // 如果+1成了
                                if(g.graphArray[temp].previous[k] == 1){
                                    for(int d = 0; d < tp.digitList.size() && d != j; d++){
                                        int temp2 = (int)tp.digitList.get(d);
                                        for(int b = 0; b < g.graphArray[temp2].previous.length; b++){
                                            if(g.graphArray[temp2].previous[b] == 1){
                                                numList[i].digitList.set(j, k);
                                                numList[i].digitList.set(d, b);
                                                numList[i].RenewValue();
                                                for(int p = 0; p < numberList.length; p++){
                                                    numberList[p] = numList[p].number;
                                                }
                                                result = numList[numberList.length].number;
                                                Equation a = new Equation(this.numberList,this.opList,this.result);
                                                this.movResult.add(a);
                                                //还原this
                                                for(int p = 0; p < this.numberList.length; p++) this.numberList[p] = bknl[p];
                                                for(int p = 0; p < this.opList.length; p++) this.opList[p] = bkop[p];
                                                this.result = bkrs;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        //添加
        else if(mode == 3){
            // 添加一根火柴
            if(matchNum == 1){
                // 首先从第一个数字开始尝试
                for(int i = 0; i < this.numberList.length + 1; i++){
                    Numeric[] numList = new Numeric[this.numberList.length + 1];
                    for(int s = 0; s < this.numberList.length; s++){
                        // 如果大于10需要额外解析
                        numList[s] = new Numeric(this.numberList[s]);
                    }
                    numList[this.numberList.length] = new Numeric(result);
                    Numeric tp = numList[i];
                    Numeric tp2 = new Numeric(numList[i].number);
                    for(int j = 0; j < tp.digitList.size(); j++){
                        for(int m = 0; m < tp.digitList.size(); m++){
                            tp.digitList.set(m,tp2.digitList.get(m));
                        }
                        tp.RenewValue();
                        int temp = (int)tp.digitList.get(j);
                        if(g.graphArray[temp].next.length != 0){
                            for(int k = 0; k < g.graphArray[temp].next.length; k++){
                                if(g.graphArray[temp].next[k] == 1){
                                    numList[i].digitList.set(j, k);
                                    numList[i].RenewValue();
                                    for(int p = 0; p < this.numberList.length; p++){
                                        numberList[p] = numList[p].number;
                                    }
                                    result = numList[this.numberList.length].number;
                                    Equation a = new Equation(this.numberList,this.opList,this.result);
                                    this.movResult.add(a);
                                    //还原this
                                    for(int p = 0; p < this.numberList.length; p++) this.numberList[p] = bknl[p];
                                    for(int p = 0; p < this.opList.length; p++) this.opList[p] = bkop[p];
                                    System.out.println(bkrs);
                                    this.result = bkrs;
                                }
                            }
                        }
                    }
                }
                //然后尝试添加加减号
                for(int i = 0; i < opList.length; i++){
                    if(opList[i] == Operation.MINUS){
                        opList[i] = Operation.PLUS;
                        Equation a = new Equation(this.numberList,this.opList,this.result);
                        this.movResult.add(a);
                        //还原this
                        for(int p = 0; p < this.numberList.length; p++) this.numberList[p] = bknl[p];
                        for(int p = 0; p < this.opList.length; p++) this.opList[p] = bkop[p];
                        this.result = bkrs;
                    }
                } 
            }
            else if(matchNum == 2){
                //第一种情况是单个变两个
                for(int i = 0; i < numberList.length + 1; i++){
                    Numeric[] numList = new Numeric[numberList.length + 1];
                    for(int s = 0; s < numberList.length; s++){
                        // 如果大于10需要额外解析
                        numList[s] = new Numeric(numberList[s]);
                    }
                    numList[numberList.length] = new Numeric(result);
                    Numeric tp = numList[i];
                    Numeric tp2 = new Numeric(numList[i].number);
                    for(int j = 0; j < tp.digitList.size(); j++){
                        for(int m = 0; m < tp.digitList.size(); m++){
                            tp.digitList.set(m,tp2.digitList.get(m));
                        }
                        tp.RenewValue();
                        int temp = (int)tp.digitList.get(j);
                        if(g.graphArray[temp].next.length != 0){
                            for(int k = 0; k < g.graphArray[temp].next.length; k++){
                                if(g.graphArray[temp].next[k] == 2){
                                    numList[i].digitList.set(j, k);
                                    numList[i].RenewValue();
                                    for(int p = 0; p < numberList.length; p++){
                                        numberList[p] = numList[p].number;
                                    }
                                    result = numList[numberList.length].number;
                                    Equation a = new Equation(this.numberList,this.opList,this.result);
                                    this.movResult.add(a);
                                    //还原this
                                    for(int p = 0; p < this.numberList.length; p++) this.numberList[p] = bknl[p];
                                    for(int p = 0; p < this.opList.length; p++) this.opList[p] = bkop[p];
                                    this.result = bkrs;
                                }
                            }
                        }
                    }
                }
                //第二种情况是两个各变一个
                for(int i = 0; i < numberList.length + 1; i++){
                    Numeric[] numList = new Numeric[numberList.length + 1];
                    for(int s = 0; s < numberList.length; s++){
                        // 如果大于10需要额外解析
                        numList[s] = new Numeric(numberList[s]);
                    }
                    numList[numberList.length] = new Numeric(result);
                    Numeric tp = numList[i];
                    Numeric tp2 = new Numeric(numList[i].number);
                    for(int j = 0; j < tp.digitList.size(); j++){
                        for(int m = 0; m < tp.digitList.size(); m++){
                            tp.digitList.set(m,tp2.digitList.get(m));
                        }
                        tp.RenewValue();
                        int temp = (int)tp.digitList.get(j);
                        if(g.graphArray[temp].next.length != 0){
                            for(int k = 0; k < g.graphArray[temp].next.length; k++){
                                // 如果+1成了
                                if(g.graphArray[temp].next[k] == 1){
                                    for(int d = 0; d < tp.digitList.size() && d != j; d++){
                                        int temp2 = (int)tp.digitList.get(d);
                                        for(int b = 0; b < g.graphArray[temp2].previous.length; b++){
                                            if(g.graphArray[temp2].next[b] == 1){
                                                numList[i].digitList.set(j, k);
                                                numList[i].digitList.set(d, b);
                                                numList[i].RenewValue();
                                                for(int p = 0; p < numberList.length; p++){
                                                    numberList[p] = numList[p].number;
                                                }
                                                result = numList[numberList.length].number;
                                                Equation a = new Equation(this.numberList,this.opList,this.result);
                                                this.movResult.add(a);
                                                //还原this
                                                for(int p = 0; p < this.numberList.length; p++) this.numberList[p] = bknl[p];
                                                for(int p = 0; p < this.opList.length; p++) this.opList[p] = bkop[p];
                                                this.result = bkrs;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else{}
        }
        else{
            System.out.println("Invalid mode");
        }

    }
}
class Numeric{
    int number;
    ArrayList digitList;
    Numeric(int num){
       // 获取位数
       int bit = 10;
       digitList = new ArrayList();
       number = num;
       while(number / bit != 0) bit *= 10;
       // 位数是多一位的，需要除以10来还原
       bit /= 10;
       while(bit != 0){
           digitList.add(num / bit);
           num = num % bit;
           bit /= 10;
       }
    }
    void RenewValue(){
        int temp = 0;
        for(int i = 0; i < digitList.size(); i++){
            temp *= 10;
            temp += (int)digitList.get(i);
        }
        number = temp;
    }
}
class Graph{
    static int[] LED = {119,18,93,91,58,107,111,82,127,123}; //对应的LED显示二进制表
    Node[] graphArray;
    Graph(){
        graphArray = new Node[10];
        for(int i = 0; i < 10; i++){
            graphArray[i] = new Node();
        }
        for(int i = 0; i < 10; i++){
            graphArray[i].number = i;
        }
    }
    void BuildGraph(){
        for(int i=0;i<10;i++){
            int bin = LED[i];
            for(int j = 64; j >= 1; j /= 2){
                // System.out.println(Integer.toBinaryString(j));
                int temp = bin & j;
                if(temp == 0){//该位是空的
                    // 尝试在此增添一根火柴棍
                    int newBin = bin | j;
                    // 如果本次添加构成了一个成功的数
                    int position = -1;
                    for(int q = 0; q < 10; q++){
                        if(LED[q] == newBin){
                            position = q;
                            break;
                        }
                    }
                    // 如果找到
                    /* if(j == 32 && i == 3){
                        System.out.println("确实探测到了 "+newBin + " " + position);
                    } */
                    if(position >= 0){
                        //连接，添加前序和后序
                        graphArray[i].next[position] = 1;
                        graphArray[position].previous[i] = 1;
                    }
                    else{
                        //这会获得一个残缺的表，我们依然要对他进行尝试，以确定是不是可以通过两次变换达到
                        for(int k = 64; k >= 1; k /= 2){
                            int temp2 = newBin & k;
                            if(temp2 == 0){
                                int verynewBin = newBin | k;
                                int position2 = Arrays.binarySearch(LED,verynewBin);
                                if(position2 >= 0){
                                    graphArray[i].next[position2] = 2;
                                    graphArray[position2].previous[i] = 2;
                                    //这样的话，差距两个的表我们也能把他联系起来了
                                    //这就可以实现移动两个火柴棒的情况
                                }
                                else{}
                            }
                        }
                    }
                }  
                else{}
            } 
        }
        //这之后再把路径为2的叠一层
        for(int i=0;i<10;i++){
            for(int j = 0; j < 10; j++){
                if(graphArray[i].next[j] == 1){
                    for(int k = 0; k < 10; k++){
                        if(graphArray[j].next[k] == 1 && k != i){
                                graphArray[i].next[k] = 2;
                                graphArray[k].previous[i] = 2;
                            }
                        }
                    }
                }
        }
        //这之后再以3为标识把自变1叠一波
        for(int i=0;i<10;i++){
            for(int j = 0; j < 10; j++){
                if(graphArray[i].next[j] == 1){
                    for(int k = 0; k < 10; k++){
                        if(graphArray[j].previous[k] == 1 && k != i){
                            graphArray[i].next[k] = 3;
                            graphArray[k].next[i] = 3;
                        }
                    }
                }
            }
        }
        //这之后再以4为标识把自变2叠一波
        for(int i=0;i<10;i++){
            for(int j = 0; j < 10; j++){
                if(graphArray[i].next[j] == 2){
                    for(int k = 0; k < 10; k++){
                        if(graphArray[j].previous[k] == 2 && k != i){
                            graphArray[i].next[k] = 4;
                            graphArray[k].next[i] = 4;
                        }
                    }
                }
            }
        }
    }
}
class Node{
    int number;
    int[] previous;
    int[] next;
    Node(){
        int number = 0;
        previous = new int[10];
        next = new int[10];
        for(int i = 0; i < 10; i++){
            previous[i] = 0;
            next[i] = 0;
        }
    }
}
class EquationMap{
    Equation eq;
    String[] output;
    EquationMap(Equation a){
        output = new String[7];
        for(int i =0; i<7;i++){
            output[i] = ""; //清空字符串
        }
        eq = new Equation(a);
        Numeric[] num = new Numeric[eq.numberList.length + 1];
        for(int i = 0; i < eq.numberList.length; i++){
            num[i] = new Numeric(eq.numberList[i]);
        } 
        num[eq.numberList.length] = new Numeric(eq.result);
        for(int i = 0; i < num.length - 1; i++){
            for(int j = 0; j < num[i].digitList.size(); j++){
                int di = (int)num[i].digitList.get(j);
                DigitMap d = new DigitMap(di);
                for(int k =0; k<7;k++){
                    output[k] += d.s[k];
                }
            }
            //输出运算符
            if(i != eq.opList.length){
                OpMap o = new OpMap(eq.opList[i]);
                for(int k =0; k<7;k++){
                    output[k] += o.s[k];
                }
            }else{
                OpMap o = new OpMap(Operation.EQUAL);
                for(int k =0; k<7;k++){
                    output[k] += o.s[k];
                }
            }
        }
        // 输出结果
        for(int j = 0; j < num[num.length - 1].digitList.size(); j++){
            int di = (int)num[num.length - 1].digitList.get(j);
            DigitMap d = new DigitMap(di);
            for(int k =0; k<7;k++){
                output[k] += d.s[k];
            }
        }
    }
    void Exhibit(){
        for(int k =0; k<7;k++){
            System.out.println(output[k]);
        }
    }
}
class DigitMap{
    int digit;
    String[] s;
    DigitMap(int di){
        s = new String[7];
        digit = di;
        switch(digit){
            case 0:
            {
                s[0] = " ══════  ";
                s[1] = "║      ║ ";
                s[2] = "║      ║ ";
                s[3] = "         ";
                s[4] = "║      ║ ";
                s[5] = "║      ║ ";
                s[6] = " ══════  ";
                break;
            }
            case 1:
            {
                s[0] = "         ";
                s[1] = "       ║ ";
                s[2] = "       ║ ";
                s[3] = "         ";
                s[4] = "       ║ ";
                s[5] = "       ║ ";
                s[6] = "         ";
                break;
            }
            case 2:
            {
                s[0] = " ══════  ";
                s[1] = "       ║ ";
                s[2] = "       ║ ";
                s[3] = " ══════  ";
                s[4] = "║        ";
                s[5] = "║        ";
                s[6] = " ══════  ";
                break;
            }
            case 3:
            {
                s[0] = " ══════  ";
                s[1] = "       ║ ";
                s[2] = "       ║ ";
                s[3] = " ══════  ";
                s[4] = "       ║ ";
                s[5] = "       ║ ";
                s[6] = " ══════  ";
                break;
            }
            case 4:
            {
                s[0] = "         ";
                s[1] = "║      ║ ";
                s[2] = "║      ║ ";
                s[3] = " ══════  ";
                s[4] = "       ║ ";
                s[5] = "       ║ ";
                s[6] = "         ";
                break;
            }
            case 5:
            {
                s[0] = " ══════  ";
                s[1] = "║        ";
                s[2] = "║        ";
                s[3] = " ══════  ";
                s[4] = "       ║ ";
                s[5] = "       ║ ";
                s[6] = " ══════  ";
                break;
            }
            case 6:
            {
                s[0] = " ══════  ";
                s[1] = "║        ";
                s[2] = "║        ";
                s[3] = " ══════  ";
                s[4] = "║      ║ ";
                s[5] = "║      ║ ";
                s[6] = " ══════  ";
                break;
            }
            case 7:
            {
                s[0] = " ══════  ";
                s[1] = "       ║ ";
                s[2] = "       ║ ";
                s[3] = "         ";
                s[4] = "       ║ ";
                s[5] = "       ║ ";
                s[6] = "         ";
                break;
            }
            case 8:
            {
                s[0] = " ══════  ";
                s[1] = "║      ║ ";
                s[2] = "║      ║ ";
                s[3] = " ══════  ";
                s[4] = "║      ║ ";
                s[5] = "║      ║ ";
                s[6] = " ══════  ";
                break;
            }
            case 9:
            {
                s[0] = " ══════  ";
                s[1] = "║      ║ ";
                s[2] = "║      ║ ";
                s[3] = " ══════  ";
                s[4] = "       ║ ";
                s[5] = "       ║ ";
                s[6] = " ══════  ";
                break;
            }

        }
    }
}

class OpMap{
    Operation op;
    String[] s;
    OpMap(Operation o){
        op = o;
        s = new String[7];
        if(op == Operation.PLUS){
            s[0] = "         ";
            s[1] = "         ";
            s[2] = "    ║    ";
            s[3] = "  ══╬══  ";
            s[4] = "    ║    ";
            s[5] = "         ";
            s[6] = "         ";
        }else if(op == Operation.MINUS){
            s[0] = "         ";
            s[1] = "         ";
            s[2] = "         ";
            s[3] = "  ═════  ";
            s[4] = "         ";
            s[5] = "         ";
            s[6] = "         ";
        }else if(op == Operation.EQUAL){
            s[0] = "         ";
            s[1] = "         ";
            s[2] = "  ═════  ";
            s[3] = "         ";
            s[4] = "  ═════  ";
            s[5] = "         ";
            s[6] = "         ";
        }
    }
}