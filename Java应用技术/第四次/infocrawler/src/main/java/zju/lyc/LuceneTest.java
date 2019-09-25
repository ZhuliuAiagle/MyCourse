package zju.lyc;

import java.io.*;
import java.util.LinkedList;
import java.util.Scanner;

import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.TokenStream;

import org.apache.lucene.analysis.tokenattributes.CharTermAttribute;
import org.apache.lucene.document.Document;
import org.apache.lucene.document.Field;
import org.apache.lucene.document.StringField;
import org.apache.lucene.document.TextField;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.index.IndexWriterConfig;
import org.apache.lucene.index.Term;
import org.apache.lucene.queryparser.classic.ParseException;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.TermQuery;
import org.apache.lucene.search.TopDocs;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.util.Version;
import org.wltea.analyzer.lucene.IKAnalyzer;

public class LuceneTest {
    public static IndexWriter iwr;
    public static void main(String[] args) {
        LuceneTest w=new LuceneTest();
        String filePath=".\\index";//创建索引的存储目录
        System.out.println("索引建立中,请稍候...\n");
        w.createIndex(filePath);//创建索引
        System.out.println("索引建立完成.\n");
        w.search(filePath);//搜索
    }
    public void createIndex(String filePath){
        File f=new File(filePath);
        iwr=null;
        try {
            Directory dir=FSDirectory.open(f);
            Analyzer analyzer = new IKAnalyzer();
            IndexWriterConfig conf=new IndexWriterConfig(Version.LUCENE_4_10_0,analyzer);
            iwr=new IndexWriter(dir,conf);//建立IndexWriter。固定套路
            try {
                // 清空之前建立的索引
                iwr.deleteAll();
            }catch(Exception e){
                e.printStackTrace();
            }
            LinkedList<Document> docs = getAllDocument();
            //添加doc，Lucene的检索是以document为基本单位
            for(Document doc : docs)
                iwr.addDocument(doc);
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        try {
            iwr.close();
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }

    public LinkedList<Document> getAllDocument(){
        LinkedList<Document> docList = new LinkedList<>();

        int docID = 0;
        //String[] qtitles;
        //String[] qbodys;
        // 进入路径
        try
        {

            File root = new File(".\\javaio");
            File[] clusters = root.listFiles();
            for(File cluster : clusters)
            {
                File[] diseases = cluster.listFiles();
                for(File disease : diseases)
                {
                    File[] attrs = disease.listFiles();
                    String url = "";
                    String tpFile = "";
                    Document doc = new Document();
                    // 从来源文件里获取疾病来源
                    try
                    {
                        int tpchar;
                        Reader r = new InputStreamReader(new FileInputStream(".\\javaio\\" + cluster.getName() +
                                "\\" + disease.getName() + "\\source.txt"));
                        while((tpchar = r.read()) != -1)
                            tpFile  =  tpFile + (char)tpchar;
                    }
                    catch(Exception e)
                    {
                        e.printStackTrace();
                    }
                    tpFile = tpFile.trim();
                    url = tpFile;
                    // 遍历所有attr
                    for(File attr : attrs)
                    {
                        int tempchar;
                        String tempFile = "";
                        try
                        {
                            Reader reader = new InputStreamReader(new FileInputStream(attr));
                            while((tempchar = reader.read()) != -1)
                                tempFile  =  tempFile + (char)tempchar;
                        }
                        catch(Exception e)
                        {
                            e.printStackTrace();
                        }
                        // 去除文件两端的空格
                        tempFile = tempFile.trim();
                        if(attr.getName().equals("source.txt"))
                        {
                            continue;
                        }
                        // 分离问题和回答
                        // 去除特殊字符
                        tempFile = tempFile.replace('/','-');
                        tempFile = tempFile.replace('\\','-');
                        String[] mixList = tempFile.split("##");
                        for(String member : mixList)
                        {
                            member = member.trim();
                            if(!member.isEmpty())
                            {
                                String q;
                                String a;
                                // member是一个问题和答案的集合
                                int line = member.indexOf("\n");
                                // 如果没有问题题干，就是用问题范畴（条目）作为题干
                                if(line == -1)
                                {
                                    q = attr.getName();
                                    a = member;
                                }
                                else
                                {
                                    q = member.substring(0, line).trim();
                                    a = member.substring(line).trim(); // 也可以line + 1
                                }
                                // 新建一个document 并把信息导入.注意这里还没有添加来源
                                doc = new Document();
                                Field fid = new TextField("编号",Integer.toString(docID),Field.Store.YES);
                                Field fquestion = new TextField("问题",q,Field.Store.YES);
                                Field fanswer = new TextField("回答",a,Field.Store.YES);
                                Field fillName = new TextField("病名",disease.getName(),Field.Store.YES);
                                Field fcluster = new TextField("科室",cluster.getName(),Field.Store.YES);
                                Field fscope = new TextField("范畴",attr.getName(),Field.Store.YES);
                                Field furl = new TextField("来源",url,Field.Store.YES);
                                // 将信息加入document
                                doc.add(fid);
                                doc.add(fquestion);
                                doc.add(fanswer);
                                doc.add(fillName);
                                doc.add(fcluster);
                                doc.add(fscope);
                                doc.add(furl);
                                docList.add(doc);
                                // docID增加
                                docID++;
                            }
                        }
                    }
                }
            }

        } catch(Exception e){
            e.printStackTrace();
        }
        // 返回document列表
        return docList;
    }


    public void search(String filePath){
        File f=new File(filePath);
        Scanner sc = new Scanner(System.in);
        try {
            while(true){
                IndexSearcher searcher=new IndexSearcher(DirectoryReader.open(FSDirectory.open(f)));
                System.out.println("【请输入您要查找的关键字,输入!quit退出】: ");
                String en;
                while((en = sc.nextLine().trim()).isEmpty());
                String queryStr=en.trim();
                if(queryStr.equals("!quit")){
                    System.out.println("再见！");
                    return;
                }
                Analyzer analyzer = new IKAnalyzer();
                //指定field为“name”，Lucene会按照关键词搜索每个doc中的name。
                System.out.println("请选择该关键字适用的范围: 【1.问题 2. 来源 3. 回答 4. 病名 5. 科室 6. 问题种类（范畴）】");
                int key = sc.nextInt();
                String scope;
                // 选择要查找的字段
                switch(key)
                {
                    case 1:
                        scope = "问题";
                        break;
                    case 2:
                        scope = "来源";
                        break;
                    case 3:
                        scope = "回答";
                        break;
                    case 4:
                        scope = "病名";
                        break;
                    case 5:
                        scope = "科室";
                        break;
                    case 6:
                        scope = "范畴";
                        break;
                    default:
                        scope = "问题";
                        break;
                }
                System.out.print("请选择结果输出个数：");
                int key_2 = 0;
                while(key_2 <= 0) {
                    key_2 = sc.nextInt();
                }
                QueryParser parser = new QueryParser(scope, analyzer);

                Query query=parser.parse(queryStr);
                TopDocs hits=searcher.search(query,key_2);//前面几行代码也是固定套路，使用时直接改field和关键词即可
                System.out.println("\n");
                // 遍历输出结果
                for(ScoreDoc doc:hits.scoreDocs) {
                    Document d = searcher.doc(doc.doc);
                    System.out.println("问题：" + d.get("问题"));
                    System.out.println("来源：" + d.get("来源"));
                    System.out.println("回答：" + d.get("回答"));
                    System.out.println("病名：" + d.get("病名"));
                    System.out.println("科室：" + d.get("科室"));
                    System.out.println("范畴：" + d.get("范畴"));
                    System.out.println("\n");
                }
            }
        } catch (IOException | ParseException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }
}
