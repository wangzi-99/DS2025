#include <iostream>
#include <cstring>
#include <cstdio>
#include <fstream>
#include "../MySTL/stack.h"
#include "../MySTL/List.h"
#include "../MySTL/binTree.h"
#include "../MySTL/hashtable.h"
#include "../MySTL/bitmap.h"

#define N_CHAR (0x80 - 0x20)
using namespace std;

struct HuffChar
{
    char ch;
    int weight;
    HuffChar(char c = '^', int w = 0) : ch(c), weight(w) {};
    bool operator<(HuffChar const& hc) { return weight > hc.weight; }
    bool operator==(HuffChar const& hc) { return weight == hc.weight; }
};

#define HuffTree BinTree<HuffChar>
typedef List<HuffTree*> HuffForest;

typedef Bitmap HuffCode;
typedef Hashtable<char, char*> HuffTable;

int* statistic(char* sample_text)
{
    int* freq = new int[N_CHAR];
    memset(freq, 0, sizeof(int) * N_CHAR);

    // 统计字符串中字符的频率
    for (int i = 0; sample_text[i] != '\0'; i++)
    {
        char ch = sample_text[i];
        if (ch >= 0x20)
        {
            freq[ch - 0x20]++;
        }
    }

    return freq;
}

HuffForest* iniForest(int* freq)
{
    HuffForest* forest = new HuffForest;
    for (int i = 0; i < N_CHAR; i++)
    {
        // 只添加频率大于0的字符
        if (freq[i] > 0)
        {
            forest->insertAsLast(new HuffTree);
            forest->last()->data->insertAsRoot(HuffChar(0x20 + i, freq[i]));
        }
    }
    return forest;
}

HuffTree* minHChar(HuffForest* forest)
{
    // 检查森林是否为空
    if (!forest || forest->empty())
    {
        return nullptr;
    }

    ListNodePosi(HuffTree*) p = forest->first();
    ListNodePosi(HuffTree*) minChar = p;
    int minWeight = p->data->root()->data.weight;
    while (forest->valid(p = p->succ))
        if (minWeight > p->data->root()->data.weight)
        {
            minWeight = p->data->root()->data.weight;
            minChar = p;
        }
    return forest->remove(minChar);
}

HuffTree* generateTree(HuffForest* forest)
{
    // 如果森林为空，创建一个空树
    if (forest->empty())
    {
        HuffTree* tree = new HuffTree();
        tree->insertAsRoot(HuffChar('^', 0));
        return tree;
    }

    // 如果森林只有一个树，直接返回
    if (forest->size() == 1)
    {
        return forest->first()->data;
    }

    // 合并森林中的树直到只剩一棵
    while (1 < forest->size())
    {
        HuffTree* T1 = minHChar(forest);
        HuffTree* T2 = minHChar(forest);
        HuffTree* S = new HuffTree();
        S->insertAsRoot(HuffChar('^', T1->root()->data.weight + T2->root()->data.weight));
        S->attachAsLC(S->root(), T1);
        S->attachAsRC(S->root(), T2);
        forest->insertAsLast(S);
    }
    return forest->first()->data;
}

static void generateCT(Bitmap* code, int length, HuffTable* table, BinNodePosi(HuffChar) v)
{
    if (!v)
        return; // 添加空节点检查，避免空指针访问

    if (IsLeaf(*v))
    {
        // 确保为叶节点生成编码，即使长度为0
        char* bits = code->bits2string(length);
        table->put(v->data.ch, bits);
        return;
    }

    if (HasLChild(*v))
    {
        code->clear(length);
        generateCT(code, length + 1, table, v->lc);
    }

    if (HasRChild(*v))
    {
        code->set(length);
        generateCT(code, length + 1, table, v->rc);
    }
}

HuffTable* generateTable(HuffTree* tree)
{
    HuffTable* table = new HuffTable;
    Bitmap* code = new Bitmap;
    generateCT(code, 0, table, tree->root());
    release(code);
    return table;
}

int encode(HuffTable* table, Bitmap* codeString, char* s)
{
    int n = 0;
    for (size_t m = strlen(s), i = 0; i < m; i++)
    {
        char** pCharCode = table->get(s[i]);
        if (!pCharCode)
        {
            pCharCode = table->get(s[i] + 'A' - 'a');
        }
        if (!pCharCode)
        {
            pCharCode = table->get(' ');
        }

        // 检查是否找到了字符编码
        if (pCharCode && *pCharCode)
        {
            printf("%s", *pCharCode);
            for (size_t m_len = strlen(*pCharCode), j = 0; j < m_len; j++)
            {
                '1' == (*pCharCode)[j] ? codeString->set(n++) : codeString->clear(n++);
            }
        }
        else
        {
            // 如果没有找到编码，则输出一个默认值（比如对应空格的编码）
            printf("[?]");
        }
    }
    printf("\n");
    return n;
}

void decode(HuffTree* tree, Bitmap* code, int n)
{
    BinNodePosi(HuffChar) x = tree->root();
    for (int i = 0; i < n; i++)
    {
        x = code->test(i) ? x->rc : x->lc;
        if (IsLeaf(*x))
        {
            printf("%c", x->data.ch);
            x = tree->root();
        }
    }
}

int main()
{

    char text[] = "i am happy to join with you today in what will go down in history as the greatest demonstration for freedom in the history of our nation five score years ago a great american in whose symbolic shadow we stand today signed the emancipation proclamation this momentous decree came as a great beacon light of hope to millions of negro slaves who had been seared in the flames of withering injustice it came as a joyous daybreak to end the long night of bad captivity but one hundred years later the negro still is not free one hundred years later the life of the negro is still sadly crippled by the manacles of segregation and the chains of discrimination one hundred years later the negro lives on a lonely island of poverty in the midst of a vast ocean of material prosperity one hundred years later the negro is still languished in the corners of american society and finds himself an exile in his own land and so weve come here today to dramatize a shameful condition in a sense weve come to our nations capital to cash a check when the architects of our republic wrote the magnificent words of the constitution and the declaration of independence they were signing a promissory note to which every american was to fall heir this note was a promise that all men yes black men as well as white men would be guaranteed the unalienable rights of life liberty and the pursuit of happiness it is obvious today that america has defaulted on this promissory note insofar as her citizens of color are concerned instead of honoring this sacred obligation america has given the negro people a bad check a check which has come back marked insufficient funds but we refuse to believe that the bank of justice is bankrupt we refuse to believe that there are insufficient funds in the great vaults of opportunity of this nation and so weve come to cash this check a check that will give us upon demand the riches of freedom and the security of justice we have also come to this hallowed spot to remind america of the fierce urgency of now this is no time to engage in the luxury of cooling off or to take the tranquilizing drug of gradualism now is the time to make real the promises of democracy now is the time to rise from the dark and desolate valley of segregation to the sunlit path of racial justice now is the time to lift our nation from the quicksands of racial injustice to the solid rock of brotherhood now is the time to make justice a reality for all of gods children it would be fatal for the nation to overlook the urgency of the moment this sweltering summer of the negros legitimate discontent will not pass until there is an invigorating autumn of freedom and equality nineteen sixty three is not an end but a beginning and those who hope that the negro needed to blow off steam and will now be content will have a rude awakening if the nation returns to business as usual and there will be neither rest nor tranquility in america until the negro is granted his citizenship rights the whirlwinds of revolt will continue to shake the foundations of our nation until the bright day of justice emerges but there is something that i must say to my people who stand on the warm threshold which leads into the palace of justice in the process of gaining our rightful place we must not be guilty of wrongful deeds let us not seek to satisfy our thirst for freedom by drinking from the cup of bitterness and hatred we must forever conduct our struggle on the high plane of dignity and discipline we must not allow our creative protest to degenerate into physical violence again and again we must rise to the majestic heights of meeting physical force with soul force the marvelous new militancy which has engulfed the negro community must not lead us to a distrust of all white people for many of our white brothers as evidenced by their presence here today have come to realize that their destiny is tied up with our destiny and they have come to realize that their freedom is inextricably bound to our freedom we cannot walk alone and as we walk we must make the pledge that we shall always march ahead we cannot turn back there are those who are asking the devotees of civil rights when will you be satisfied we can never be satisfied as long as the negro is the victim of the unspeakable horrors of police brutality we can never be satisfied as long as our bodies heavy with the fatigue of travel cannot gain lodging in the motels of the highways and the hotels of the cities we cannot be satisfied as long as the negros basic mobility is from a smaller ghetto to a larger one we can never be satisfied as long as our children are stripped of their selfhood and robbed of their dignity by signs stating for whites only we cannot be satisfied as long as a negro in mississippi cannot vote and a negro in new york believes he has nothing for which to vote no no we are not satisfied and we will not be satisfied until justice rolls down like waters and righteousness like a mighty stream i am not unmindful that some of you have come here out of great trials and tribulations some of you have come fresh from narrow jail cells and some of you have come from areas where your quest quest for freedom left you battered by the storms of persecution and staggered by the winds of police brutality you have been the veterans of creative suffering continue to work with the faith that unearned suffering is redemptive go back to mississippi go back to alabama go back to south carolina go back to georgia go back to louisiana go back to the slums and ghettos of our northern cities knowing that somehow this situation can and will be changed let us not wallow in the valley of despair i say to you today my friends and so even though we face the difficulties of today and tomorrow i still have a dream it is a dream deeply rooted in the american dream i have a dream that one day this nation will rise up and live out the true meaning of its creed we hold these truths to be self evident that all men are created equal i have a dream that one day on the red hills of georgia the sons of former slaves and the sons of former slave owners will be able to sit down together at the table of brotherhood i have a dream that one day even the state of mississippi a state sweltering with the heat of injustice sweltering with the heat of oppression will be transformed into an oasis of freedom and justice i have a dream that my four little children will one day live in a nation where they will not be judged by the color of their skin but by the content of their character i have a dream today i have a dream that one day down in alabama with its vicious racists with its governor having his lips dripping with the words of interposition and nullification one day right there in alabama little black boys and black girls will be able to join hands with little white boys and white girls as sisters and brothers i have a dream today i have a dream that one day every valley shall be exalted and every hill and mountain shall be made low the rough places will be made plain and the crooked places will be made straight and the glory of the lord shall be revealed and all flesh shall see it together this is our hope and this is the faith that i go back to the south with with this faith we will be able to hew out of the mountain of despair a stone of hope with this faith we will be able to transform the jangling discords of our nation into a beautiful symphony of brotherhood with this faith we will be able to work together to pray together to struggle together to go to jail together to stand up for freedom together knowing that we will be free one day and this will be the day this will be the day when all of gods children will be able to sing with new meaning my country tis of thee sweet land of liberty of thee i sing land where my fathers died land of the pilgrims pride from every mountainside let freedom ring and if america is to be a great nation this must become true and so let freedom ring from the prodigious hilltops of new hampshire let freedom ring from the mighty mountains of new york let freedom ring from the heightening alleghenies of pennsylvania let freedom ring from the snow capped rockies of colorado let freedom ring from the curvaceous slopes of california but not only that let freedom ring from stone mountain of georgia let freedom ring from lookout mountain of tennessee let freedom ring from every hill and molehill of mississippi from every mountainside let freedom ring and when this happens when we allow freedom ring when we let it ring from every village and every hamlet from every state and every city we will be able to speed up that day when all of gods children black men and white men jews and gentiles protestants and catholics will be able to join hands and sing in the words of the old negro spiritual free at last free at last thank god almighty we are free at last";

    cout << "原文: " << text << endl;

    int* freq = statistic(text);
    HuffForest* forest = iniForest(freq);

    HuffTree* tree = generateTree(forest);
    HuffTable* table = generateTable(tree);

    Bitmap* codeString = new Bitmap(strlen(text) * 10);
    cout << "开始编码，结果为" << endl;
    int n = encode(table, codeString, text);
    // cout << "编码为:" << endl;
    // for (int i = 0; i < n; i++)
    // {
    //     cout << (codeString->test(i) ? 1 : 0);
    // }
    // cout << endl;

    // 清理内
    delete[] freq;
    delete codeString;
    release(table);
    release(tree);
    delete forest;
    char text2[] = "dream";

    cout << "原文: " << text2 << endl;

    freq = statistic(text2);
    forest = iniForest(freq);

    tree = generateTree(forest);
    table = generateTable(tree);

    codeString = new Bitmap(strlen(text) * 10);
    cout << "开始编码，结果为" << endl;
    n = encode(table, codeString, text2);
    delete[] freq;
    delete codeString;
    release(table);
    release(tree);
    delete forest;
    char text3[] = "pigeon";

    cout << "原文: " << text3 << endl;

    freq = statistic(text3);
    forest = iniForest(freq);

    tree = generateTree(forest);
    table = generateTable(tree);

    codeString = new Bitmap(strlen(text) * 10);
    cout << "开始编码，结果为" << endl;
    n = encode(table, codeString, text3);
    delete[] freq;
    delete codeString;
    release(table);
    release(tree);
    delete forest;
    char text4[] = "love";

    cout << "原文: " << text4 << endl;

    freq = statistic(text4);
    forest = iniForest(freq);

    tree = generateTree(forest);
    table = generateTable(tree);

    codeString = new Bitmap(strlen(text) * 10);
    cout << "开始编码，结果为" << endl;
    n = encode(table, codeString, text4);
    delete[] freq;
    delete codeString;
    release(table);
    release(tree);
    delete forest;
    return 0;
}