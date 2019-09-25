package zju.lyc;
import org.apache.http.client.HttpResponseException;
import org.jsoup.select.Elements;


import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.net.InetSocketAddress;
import java.net.Proxy;
import java.util.*;
import org.jsoup.*;
import org.jsoup.nodes.*;


/**
 * Hello world!
 *
 */
public class App 
{
    static boolean pflag = false;
    static Proxy p1 = new Proxy(Proxy.Type.HTTP, new InetSocketAddress("116.255.170.46",16819));
    static Proxy p2 = new Proxy(Proxy.Type.HTTP, new InetSocketAddress("183.196.170.247",9000));
    static String agent ="Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko)" + "  Chrome/56.0.2924.87 Safari/537.36";
    public static void main(String[] args) throws Exception {
        try{
            Document doc = new Document("xxx");
            for(;;) {
                try {
                    doc = Jsoup.connect("https://dxy.com/diseases/").proxy(pflag?p1:null).ignoreContentType(true).userAgent(agent).get();
                    break;
                } catch (Exception e) {
                    if(pflag == false){
                        pflag = true;
                        System.out.println("change route");
                        Thread.sleep(2000);
                    }
                    else{
                        pflag = false;
                        System.out.println("retry...");
                        Thread.sleep(610000);
                    }
                }
            }
            // 获取当前页面（主页）的标题
            String title = doc.title();
            // 使用class名访问科室链接所在的navbar
            Elements diseaseClusters = doc.getElementsByClass("section-navbar-item");
            System.out.println(title);
            // 对于每个科室
            int count = 0;
            // 遍历所有科室
            for(Element diseaseCluster:diseaseClusters)
            {
                count++;
                if(count < 2) continue;
                System.out.println(diseaseCluster.text());
                // 为每个科室在本地新建目录
                String path = ".\\javaio\\";
                File file = new File(".\\javaio\\" + diseaseCluster.text());
                if(!file.exists())
                {
                    file.mkdir();
                }
                // 获取当前科室链接
                Element diseaseClusterLink = diseaseCluster.select("a").first();
                // 获取当前科室链接
                String diseaseClusterHref = diseaseClusterLink.attr("href");
                Random rand = new Random();
                // 随机休眠，规避反爬策略
                Thread.sleep( rand.nextInt(1000));
                Document innerDoc = new Document("xxx");
                // 反反爬处理
                for(;;)
                {
                    try
                    {
                        // 进入科室链接，获取当前科室对应的页面内容，pflag判断此时是否使用代理
                        Connection cl = Jsoup.connect(diseaseClusterHref).proxy(pflag?p1:null).ignoreContentType(true).userAgent(agent);
                        innerDoc = cl.get();
                        break;
                    }
                    catch (Exception e)
                    {
                        // 切换线路
                        if(pflag == false)
                        {
                            pflag = true;
                            Thread.sleep(500);
                        }
                        // 两个线路全部被禁止访问，等待十分钟
                        else
                        {
                            pflag = false;
                            System.out.println("retry...");
                            Thread.sleep(610000);
                        }
                    }
                }
                //获取该科室最大页数
                int max_page = 0;
                // 定位到存储最大页数的pagination位置（虽然类型是Elements但实际上只有一个）
                Elements page_elements = innerDoc.getElementsByClass("pagination");
                for(Element page_element:page_elements)
                {
                    // 获取存储最大页码的表项内容（aria-label属性中包含page字段）
                    String page = page_element.select("[aria-label^=Page]").last().text();
                    //将表项内容解析为整型数值
                    max_page = Integer.parseInt(page);

                }
                System.out.println("max page : " + max_page);
                // 对于存在的每一页，依次请求
                for(int i = 1; i <= max_page; i++)
                {
                    // 简单的反爬虫策略
                    Thread.sleep( rand.nextInt(1000));
                    System.out.println("---Page " + i);
                    // 获取该页的信息
                    for(;;) {
                        try {
                            innerDoc = Jsoup.connect(diseaseClusterHref + "?page=" + i).ignoreContentType(true).proxy(pflag?p1:null).userAgent(agent).get();
                            break;
                        } catch (Exception e) {
                            if(pflag == false){
                                pflag = true;
                                Thread.sleep(600);
                            }
                            else{
                                pflag = false;
                                System.out.println("retry...");
                                Thread.sleep(610000);
                            }
                        }
                    }

                    Elements diseaseInfos = innerDoc.getElementsByClass("lr-container-left").select(".disease-card-info");
                    // 提取疾病信息连接，处理疾病信息
                    for(Element diseaseInfo : diseaseInfos)
                    {
                        Thread.sleep( rand.nextInt(1000));
                        String diseaseInfoHref = diseaseInfo.select("a").first().attr("href");
                        String diseaseTitle = diseaseInfo.select("a").first().text();
                        System.out.println("----" + diseaseTitle + ": " + diseaseInfoHref);
                        processSinglePage(diseaseInfoHref, path + diseaseCluster.text());

                    }
                }

            }
        }catch(HttpStatusException e){
            System.out.println("Error: 404 not found");
        }
    }

    public static void processSinglePage(String url, String path) throws Exception
    {
        try {
                Document mainPage = new Document("xxx");
                for(;;) {
                    try {
                        // 连接到对应疾病信息页面
                        mainPage = Jsoup.connect(url).ignoreContentType(true).proxy(pflag?p1:null).userAgent(agent).get();
                        break;
                    } catch (Exception e) {
                        // 切换线路
                        if(pflag == false){
                            pflag = true;
                            Thread.sleep(700);
                        }
                        // 两个线路都被禁用，等待10分钟
                        else{
                            pflag = false;
                            System.out.println("retry...");
                            Thread.sleep(610000);
                        }
                    }
                }
                // 获取当前疾病的名称
                String mainTitle = mainPage.getElementsByClass("disease-card-info-title").first().text(); // 保存为文件名用
                // 处理可能存在的异常字符
                mainTitle = mainTitle.replace('/', '-');
                System.out.println(mainTitle);
                File file = new File(path + "\\" + mainTitle);
                // 如果当前疾病对应的目录不存在，创建对应的目录
                if(!file.exists())
                {
                    file.mkdir();
                }
                //
                // 获取疾病基本信息
                Element firstIntro = mainPage.getElementsByClass("disease-card-info-content").first();
                System.out.println("--------" + firstIntro.text());
                // 解析条目列表
                Elements attributeList = mainPage.getElementsByClass("disease-detail-card");
                // 对于每一个条目
                for(Element attribute : attributeList)
                {
                    // 解析条目标题
                    String attrTitle = attribute.select(".disease-detail-card-title").first().text();
                    // 解析条目内容
                    String attrContent = attribute.select(".disease-detail-card-deatil").first().text();
                    // 写入文件
                    //attrContent = attrContent.replace(' ', '\n');

                    String attrText = "";
                    Element attrElement = attribute.select(".disease-detail-card-deatil").first();
                    Elements attrSingles = attrElement.children();
                    // 对于每一个条目下面的Q/A
                    for(Element child : attrSingles)
                    {
                        // 如果是问题题干，在前面加上特殊标识
                        if(child.is("h2"))
                        {
                            attrText += ("\n##Q:" + child.text() + "\n");
                        }
                        // 如果是普通内容
                        else
                        {
                            attrText += (child.text() + "\n");
                        }
                    }
                    // 在问题最末尾加上结束标识
                    attrText += "##";
                    BufferedWriter bw=new BufferedWriter(new FileWriter(path + "\\" + mainTitle + "\\" + attrTitle + ".txt"));
                    // 将结构化的条目内容写入对应路径下的对应文本文件中
                    bw.write(attrText);
                    bw.close();
                }
                // 将来源写入对应文件夹的source.txt中存储
                BufferedWriter bw=new BufferedWriter(new FileWriter(path + "\\" + mainTitle + "\\" + "source.txt"));
                bw.write(url + "\n");
                // 关闭文件
                bw.close();
        }catch(HttpStatusException e){
            e.printStackTrace();
        }
    }
}

