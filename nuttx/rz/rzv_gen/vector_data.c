/* generated vector source file - do not edit */
#include "bsp_api.h"
#include "vector_data.h"

extern void Default_Handler(void);
#if defined(BSP_SUPPORT_CORE_CR8)
BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_MAX_ENTRIES+BSP_CORTEX_VECTOR_TABLE_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
{
    Default_Handler, /* IRQ:-32  */
    Default_Handler, /* IRQ:-31  */
    Default_Handler, /* IRQ:-30  */
    Default_Handler, /* IRQ:-29  */
    Default_Handler, /* IRQ:-28  */
    Default_Handler, /* IRQ:-27  */
    Default_Handler, /* IRQ:-26  */
    Default_Handler, /* IRQ:-25  */
    Default_Handler, /* IRQ:-24  */
    Default_Handler, /* IRQ:-23  */
    Default_Handler, /* IRQ:-22  */
    Default_Handler, /* IRQ:-21  */
    Default_Handler, /* IRQ:-20  */
    Default_Handler, /* IRQ:-19  */
    Default_Handler, /* IRQ:-18  */
    Default_Handler, /* IRQ:-17  */
    Default_Handler, /* IRQ:-16  */
    Default_Handler, /* IRQ:-15  */
    Default_Handler, /* IRQ:-14  */
    Default_Handler, /* IRQ:-13  */
    Default_Handler, /* IRQ:-12  */
    Default_Handler, /* IRQ:-11  */
    Default_Handler, /* IRQ:-10  */
    Default_Handler, /* IRQ:-9  */
    Default_Handler, /* IRQ:-8  */
    Default_Handler, /* IRQ:-7  */
    Default_Handler, /* IRQ:-6  */
    Default_Handler, /* IRQ:-5  */
    Default_Handler, /* IRQ:-4  */
    Default_Handler, /* IRQ:-3  */
    Default_Handler, /* IRQ:-2  */
    Default_Handler, /* IRQ:-1  */
#else
BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_MAX_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
{
#endif
    Default_Handler, /* IRQ:0  */
    Default_Handler, /* IRQ:1  */
    Default_Handler, /* IRQ:2  */
    Default_Handler, /* IRQ:3  */
    Default_Handler, /* IRQ:4  */
    Default_Handler, /* IRQ:5  */
    Default_Handler, /* IRQ:6  */
    Default_Handler, /* IRQ:7  */
    Default_Handler, /* IRQ:8  */
    Default_Handler, /* IRQ:9  */
    Default_Handler, /* IRQ:10  */
    Default_Handler, /* IRQ:11  */
    Default_Handler, /* IRQ:12  */
    Default_Handler, /* IRQ:13  */
    Default_Handler, /* IRQ:14  */
    Default_Handler, /* IRQ:15  */
    Default_Handler, /* IRQ:16  */
    Default_Handler, /* IRQ:17  */
    Default_Handler, /* IRQ:18  */
    Default_Handler, /* IRQ:19  */
    Default_Handler, /* IRQ:20  */
    Default_Handler, /* IRQ:21  */
    Default_Handler, /* IRQ:22  */
    Default_Handler, /* IRQ:23  */
    Default_Handler, /* IRQ:24  */
    Default_Handler, /* IRQ:25  */
    Default_Handler, /* IRQ:26  */
    Default_Handler, /* IRQ:27  */
    Default_Handler, /* IRQ:28  */
    Default_Handler, /* IRQ:29  */
    Default_Handler, /* IRQ:30  */
    Default_Handler, /* IRQ:31  */
    Default_Handler, /* IRQ:32  */
    Default_Handler, /* IRQ:33  */
    Default_Handler, /* IRQ:34  */
    Default_Handler, /* IRQ:35  */
    Default_Handler, /* IRQ:36  */
    Default_Handler, /* IRQ:37  */
    Default_Handler, /* IRQ:38  */
    Default_Handler, /* IRQ:39  */
    Default_Handler, /* IRQ:40  */
    Default_Handler, /* IRQ:41  */
    Default_Handler, /* IRQ:42  */
    Default_Handler, /* IRQ:43  */
    Default_Handler, /* IRQ:44  */
    Default_Handler, /* IRQ:45  */
    Default_Handler, /* IRQ:46  */
    Default_Handler, /* IRQ:47  */
    Default_Handler, /* IRQ:48  */
    Default_Handler, /* IRQ:49  */
    Default_Handler, /* IRQ:50  */
    Default_Handler, /* IRQ:51  */
    Default_Handler, /* IRQ:52  */
    Default_Handler, /* IRQ:53  */
    Default_Handler, /* IRQ:54  */
    Default_Handler, /* IRQ:55  */
    Default_Handler, /* IRQ:56  */
    Default_Handler, /* IRQ:57  */
    Default_Handler, /* IRQ:58  */
    Default_Handler, /* IRQ:59  */
    Default_Handler, /* IRQ:60  */
    Default_Handler, /* IRQ:61  */
    Default_Handler, /* IRQ:62  */
    Default_Handler, /* IRQ:63  */
    Default_Handler, /* IRQ:64  */
    Default_Handler, /* IRQ:65  */
    Default_Handler, /* IRQ:66  */
    Default_Handler, /* IRQ:67  */
    Default_Handler, /* IRQ:68  */
    Default_Handler, /* IRQ:69  */
    Default_Handler, /* IRQ:70  */
    Default_Handler, /* IRQ:71  */
    Default_Handler, /* IRQ:72  */
    Default_Handler, /* IRQ:73  */
    Default_Handler, /* IRQ:74  */
    Default_Handler, /* IRQ:75  */
    Default_Handler, /* IRQ:76  */
    Default_Handler, /* IRQ:77  */
    Default_Handler, /* IRQ:78  */
    Default_Handler, /* IRQ:79  */
    Default_Handler, /* IRQ:80  */
    Default_Handler, /* IRQ:81  */
    Default_Handler, /* IRQ:82  */
    Default_Handler, /* IRQ:83  */
    Default_Handler, /* IRQ:84  */
    Default_Handler, /* IRQ:85  */
    Default_Handler, /* IRQ:86  */
    Default_Handler, /* IRQ:87  */
    Default_Handler, /* IRQ:88  */
    Default_Handler, /* IRQ:89  */
    Default_Handler, /* IRQ:90  */
    Default_Handler, /* IRQ:91  */
    Default_Handler, /* IRQ:92  */
    Default_Handler, /* IRQ:93  */
    Default_Handler, /* IRQ:94  */
    Default_Handler, /* IRQ:95  */
    Default_Handler, /* IRQ:96  */
    Default_Handler, /* IRQ:97  */
    Default_Handler, /* IRQ:98  */
    Default_Handler, /* IRQ:99  */
    Default_Handler, /* IRQ:100  */
    Default_Handler, /* IRQ:101  */
    Default_Handler, /* IRQ:102  */
    Default_Handler, /* IRQ:103  */
    Default_Handler, /* IRQ:104  */
    Default_Handler, /* IRQ:105  */
    Default_Handler, /* IRQ:106  */
    Default_Handler, /* IRQ:107  */
    Default_Handler, /* IRQ:108  */
    Default_Handler, /* IRQ:109  */
    Default_Handler, /* IRQ:110  */
    Default_Handler, /* IRQ:111  */
    Default_Handler, /* IRQ:112  */
    Default_Handler, /* IRQ:113  */
    Default_Handler, /* IRQ:114  */
    Default_Handler, /* IRQ:115  */
    Default_Handler, /* IRQ:116  */
    Default_Handler, /* IRQ:117  */
    Default_Handler, /* IRQ:118  */
    Default_Handler, /* IRQ:119  */
    Default_Handler, /* IRQ:120  */
    Default_Handler, /* IRQ:121  */
    Default_Handler, /* IRQ:122  */
    Default_Handler, /* IRQ:123  */
    Default_Handler, /* IRQ:124  */
    Default_Handler, /* IRQ:125  */
    Default_Handler, /* IRQ:126  */
    Default_Handler, /* IRQ:127  */
    Default_Handler, /* IRQ:128  */
    Default_Handler, /* IRQ:129  */
    Default_Handler, /* IRQ:130  */
    Default_Handler, /* IRQ:131  */
    Default_Handler, /* IRQ:132  */
    Default_Handler, /* IRQ:133  */
    Default_Handler, /* IRQ:134  */
    Default_Handler, /* IRQ:135  */
    Default_Handler, /* IRQ:136  */
    Default_Handler, /* IRQ:137  */
    Default_Handler, /* IRQ:138  */
    Default_Handler, /* IRQ:139  */
    Default_Handler, /* IRQ:140  */
    Default_Handler, /* IRQ:141  */
    Default_Handler, /* IRQ:142  */
    Default_Handler, /* IRQ:143  */
    Default_Handler, /* IRQ:144  */
    Default_Handler, /* IRQ:145  */
    Default_Handler, /* IRQ:146  */
    Default_Handler, /* IRQ:147  */
    Default_Handler, /* IRQ:148  */
    Default_Handler, /* IRQ:149  */
    Default_Handler, /* IRQ:150  */
    Default_Handler, /* IRQ:151  */
    Default_Handler, /* IRQ:152  */
    Default_Handler, /* IRQ:153  */
    Default_Handler, /* IRQ:154  */
    Default_Handler, /* IRQ:155  */
    Default_Handler, /* IRQ:156  */
    Default_Handler, /* IRQ:157  */
    Default_Handler, /* IRQ:158  */
    Default_Handler, /* IRQ:159  */
    Default_Handler, /* IRQ:160  */
    Default_Handler, /* IRQ:161  */
    Default_Handler, /* IRQ:162  */
    Default_Handler, /* IRQ:163  */
    Default_Handler, /* IRQ:164  */
    Default_Handler, /* IRQ:165  */
    Default_Handler, /* IRQ:166  */
    Default_Handler, /* IRQ:167  */
    Default_Handler, /* IRQ:168  */
    Default_Handler, /* IRQ:169  */
    Default_Handler, /* IRQ:170  */
    Default_Handler, /* IRQ:171  */
    Default_Handler, /* IRQ:172  */
    Default_Handler, /* IRQ:173  */
    Default_Handler, /* IRQ:174  */
    Default_Handler, /* IRQ:175  */
    Default_Handler, /* IRQ:176  */
    Default_Handler, /* IRQ:177  */
    Default_Handler, /* IRQ:178  */
    Default_Handler, /* IRQ:179  */
    Default_Handler, /* IRQ:180  */
    Default_Handler, /* IRQ:181  */
    Default_Handler, /* IRQ:182  */
    Default_Handler, /* IRQ:183  */
    Default_Handler, /* IRQ:184  */
    Default_Handler, /* IRQ:185  */
    Default_Handler, /* IRQ:186  */
    Default_Handler, /* IRQ:187  */
    Default_Handler, /* IRQ:188  */
    Default_Handler, /* IRQ:189  */
    Default_Handler, /* IRQ:190  */
    Default_Handler, /* IRQ:191  */
    Default_Handler, /* IRQ:192  */
    Default_Handler, /* IRQ:193  */
    Default_Handler, /* IRQ:194  */
    Default_Handler, /* IRQ:195  */
    Default_Handler, /* IRQ:196  */
    Default_Handler, /* IRQ:197  */
    Default_Handler, /* IRQ:198  */
    Default_Handler, /* IRQ:199  */
    Default_Handler, /* IRQ:200  */
    Default_Handler, /* IRQ:201  */
    Default_Handler, /* IRQ:202  */
    Default_Handler, /* IRQ:203  */
    Default_Handler, /* IRQ:204  */
    Default_Handler, /* IRQ:205  */
    Default_Handler, /* IRQ:206  */
    Default_Handler, /* IRQ:207  */
    Default_Handler, /* IRQ:208  */
    Default_Handler, /* IRQ:209  */
    Default_Handler, /* IRQ:210  */
    Default_Handler, /* IRQ:211  */
    Default_Handler, /* IRQ:212  */
    Default_Handler, /* IRQ:213  */
    Default_Handler, /* IRQ:214  */
    Default_Handler, /* IRQ:215  */
    Default_Handler, /* IRQ:216  */
    Default_Handler, /* IRQ:217  */
    Default_Handler, /* IRQ:218  */
    Default_Handler, /* IRQ:219  */
    Default_Handler, /* IRQ:220  */
    Default_Handler, /* IRQ:221  */
    Default_Handler, /* IRQ:222  */
    Default_Handler, /* IRQ:223  */
    Default_Handler, /* IRQ:224  */
    Default_Handler, /* IRQ:225  */
    Default_Handler, /* IRQ:226  */
    Default_Handler, /* IRQ:227  */
    Default_Handler, /* IRQ:228  */
    Default_Handler, /* IRQ:229  */
    Default_Handler, /* IRQ:230  */
    Default_Handler, /* IRQ:231  */
    Default_Handler, /* IRQ:232  */
    Default_Handler, /* IRQ:233  */
    Default_Handler, /* IRQ:234  */
    Default_Handler, /* IRQ:235  */
    Default_Handler, /* IRQ:236  */
    Default_Handler, /* IRQ:237  */
    Default_Handler, /* IRQ:238  */
    Default_Handler, /* IRQ:239  */
    Default_Handler, /* IRQ:240  */
    Default_Handler, /* IRQ:241  */
    Default_Handler, /* IRQ:242  */
    Default_Handler, /* IRQ:243  */
    Default_Handler, /* IRQ:244  */
    Default_Handler, /* IRQ:245  */
    Default_Handler, /* IRQ:246  */
    Default_Handler, /* IRQ:247  */
    Default_Handler, /* IRQ:248  */
    Default_Handler, /* IRQ:249  */
    Default_Handler, /* IRQ:250  */
    Default_Handler, /* IRQ:251  */
    Default_Handler, /* IRQ:252  */
    Default_Handler, /* IRQ:253  */
    Default_Handler, /* IRQ:254  */
    Default_Handler, /* IRQ:255  */
    Default_Handler, /* IRQ:256  */
    Default_Handler, /* IRQ:257  */
    Default_Handler, /* IRQ:258  */
    Default_Handler, /* IRQ:259  */
    Default_Handler, /* IRQ:260  */
    Default_Handler, /* IRQ:261  */
    Default_Handler, /* IRQ:262  */
    Default_Handler, /* IRQ:263  */
    Default_Handler, /* IRQ:264  */
    Default_Handler, /* IRQ:265  */
    Default_Handler, /* IRQ:266  */
    Default_Handler, /* IRQ:267  */
    Default_Handler, /* IRQ:268  */
    Default_Handler, /* IRQ:269  */
    Default_Handler, /* IRQ:270  */
    Default_Handler, /* IRQ:271  */
    Default_Handler, /* IRQ:272  */
    Default_Handler, /* IRQ:273  */
    Default_Handler, /* IRQ:274  */
    Default_Handler, /* IRQ:275  */
    Default_Handler, /* IRQ:276  */
    Default_Handler, /* IRQ:277  */
    Default_Handler, /* IRQ:278  */
    Default_Handler, /* IRQ:279  */
    Default_Handler, /* IRQ:280  */
    Default_Handler, /* IRQ:281  */
    Default_Handler, /* IRQ:282  */
    Default_Handler, /* IRQ:283  */
    Default_Handler, /* IRQ:284  */
    Default_Handler, /* IRQ:285  */
    Default_Handler, /* IRQ:286  */
    Default_Handler, /* IRQ:287  */
    Default_Handler, /* IRQ:288  */
    Default_Handler, /* IRQ:289  */
    Default_Handler, /* IRQ:290  */
    Default_Handler, /* IRQ:291  */
    Default_Handler, /* IRQ:292  */
    Default_Handler, /* IRQ:293  */
    Default_Handler, /* IRQ:294  */
    Default_Handler, /* IRQ:295  */
    Default_Handler, /* IRQ:296  */
    Default_Handler, /* IRQ:297  */
    Default_Handler, /* IRQ:298  */
    Default_Handler, /* IRQ:299  */
    Default_Handler, /* IRQ:300  */
    Default_Handler, /* IRQ:301  */
    Default_Handler, /* IRQ:302  */
    Default_Handler, /* IRQ:303  */
    Default_Handler, /* IRQ:304  */
    Default_Handler, /* IRQ:305  */
    Default_Handler, /* IRQ:306  */
    Default_Handler, /* IRQ:307  */
    Default_Handler, /* IRQ:308  */
    Default_Handler, /* IRQ:309  */
    Default_Handler, /* IRQ:310  */
    Default_Handler, /* IRQ:311  */
    Default_Handler, /* IRQ:312  */
    Default_Handler, /* IRQ:313  */
    Default_Handler, /* IRQ:314  */
    Default_Handler, /* IRQ:315  */
    Default_Handler, /* IRQ:316  */
    Default_Handler, /* IRQ:317  */
    Default_Handler, /* IRQ:318  */
    Default_Handler, /* IRQ:319  */
    Default_Handler, /* IRQ:320  */
    Default_Handler, /* IRQ:321  */
    Default_Handler, /* IRQ:322  */
    Default_Handler, /* IRQ:323  */
    Default_Handler, /* IRQ:324  */
    Default_Handler, /* IRQ:325  */
    Default_Handler, /* IRQ:326  */
    Default_Handler, /* IRQ:327  */
    Default_Handler, /* IRQ:328  */
    Default_Handler, /* IRQ:329  */
    Default_Handler, /* IRQ:330  */
    Default_Handler, /* IRQ:331  */
    Default_Handler, /* IRQ:332  */
    Default_Handler, /* IRQ:333  */
    Default_Handler, /* IRQ:334  */
    Default_Handler, /* IRQ:335  */
    Default_Handler, /* IRQ:336  */
    Default_Handler, /* IRQ:337  */
    Default_Handler, /* IRQ:338  */
    Default_Handler, /* IRQ:339  */
    Default_Handler, /* IRQ:340  */
    Default_Handler, /* IRQ:341  */
    Default_Handler, /* IRQ:342  */
    Default_Handler, /* IRQ:343  */
    Default_Handler, /* IRQ:344  */
    Default_Handler, /* IRQ:345  */
    Default_Handler, /* IRQ:346  */
    Default_Handler, /* IRQ:347  */
    Default_Handler, /* IRQ:348  */
    Default_Handler, /* IRQ:349  */
    Default_Handler, /* IRQ:350  */
    Default_Handler, /* IRQ:351  */
    Default_Handler, /* IRQ:352  */
    Default_Handler, /* IRQ:353  */
    Default_Handler, /* IRQ:354  */
    Default_Handler, /* IRQ:355  */
    Default_Handler, /* IRQ:356  */
    Default_Handler, /* IRQ:357  */
    Default_Handler, /* IRQ:358  */
    Default_Handler, /* IRQ:359  */
    Default_Handler, /* IRQ:360  */
    Default_Handler, /* IRQ:361  */
    Default_Handler, /* IRQ:362  */
    Default_Handler, /* IRQ:363  */
    Default_Handler, /* IRQ:364  */
    Default_Handler, /* IRQ:365  */
    Default_Handler, /* IRQ:366  */
    Default_Handler, /* IRQ:367  */
    Default_Handler, /* IRQ:368  */
    Default_Handler, /* IRQ:369  */
    Default_Handler, /* IRQ:370  */
    Default_Handler, /* IRQ:371  */
    Default_Handler, /* IRQ:372  */
    Default_Handler, /* IRQ:373  */
    Default_Handler, /* IRQ:374  */
    Default_Handler, /* IRQ:375  */
    Default_Handler, /* IRQ:376  */
    Default_Handler, /* IRQ:377  */
    Default_Handler, /* IRQ:378  */
    Default_Handler, /* IRQ:379  */
    Default_Handler, /* IRQ:380  */
    Default_Handler, /* IRQ:381  */
    Default_Handler, /* IRQ:382  */
    Default_Handler, /* IRQ:383  */
    Default_Handler, /* IRQ:384  */
    Default_Handler, /* IRQ:385  */
    Default_Handler, /* IRQ:386  */
    Default_Handler, /* IRQ:387  */
    Default_Handler, /* IRQ:388  */
    Default_Handler, /* IRQ:389  */
    Default_Handler, /* IRQ:390  */
    Default_Handler, /* IRQ:391  */
    Default_Handler, /* IRQ:392  */
    Default_Handler, /* IRQ:393  */
    Default_Handler, /* IRQ:394  */
    Default_Handler, /* IRQ:395  */
    Default_Handler, /* IRQ:396  */
    Default_Handler, /* IRQ:397  */
    Default_Handler, /* IRQ:398  */
    Default_Handler, /* IRQ:399  */
    Default_Handler, /* IRQ:400  */
    Default_Handler, /* IRQ:401  */
    Default_Handler, /* IRQ:402  */
    Default_Handler, /* IRQ:403  */
    Default_Handler, /* IRQ:404  */
    Default_Handler, /* IRQ:405  */
    Default_Handler, /* IRQ:406  */
    Default_Handler, /* IRQ:407  */
    Default_Handler, /* IRQ:408  */
    Default_Handler, /* IRQ:409  */
    Default_Handler, /* IRQ:410  */
    Default_Handler, /* IRQ:411  */
    Default_Handler, /* IRQ:412  */
    Default_Handler, /* IRQ:413  */
    Default_Handler, /* IRQ:414  */
    Default_Handler, /* IRQ:415  */
    Default_Handler, /* IRQ:416  */
    Default_Handler, /* IRQ:417  */
    Default_Handler, /* IRQ:418  */
    Default_Handler, /* IRQ:419  */
    Default_Handler, /* IRQ:420  */
    Default_Handler, /* IRQ:421  */
    Default_Handler, /* IRQ:422  */
    Default_Handler, /* IRQ:423  */
    Default_Handler, /* IRQ:424  */
    Default_Handler, /* IRQ:425  */
    Default_Handler, /* IRQ:426  */
    Default_Handler, /* IRQ:427  */
    Default_Handler, /* IRQ:428  */
    Default_Handler, /* IRQ:429  */
    Default_Handler, /* IRQ:430  */
    Default_Handler, /* IRQ:431  */
    Default_Handler, /* IRQ:432  */
    Default_Handler, /* IRQ:433  */
    Default_Handler, /* IRQ:434  */
    Default_Handler, /* IRQ:435  */
    Default_Handler, /* IRQ:436  */
    Default_Handler, /* IRQ:437  */
    Default_Handler, /* IRQ:438  */
    Default_Handler, /* IRQ:439  */
    Default_Handler, /* IRQ:440  */
    Default_Handler, /* IRQ:441  */
    Default_Handler, /* IRQ:442  */
    Default_Handler, /* IRQ:443  */
    Default_Handler, /* IRQ:444  */
    Default_Handler, /* IRQ:445  */
    Default_Handler, /* IRQ:446  */
    Default_Handler, /* IRQ:447  */
    Default_Handler, /* IRQ:448  */
    Default_Handler, /* IRQ:449  */
    Default_Handler, /* IRQ:450  */
    Default_Handler, /* IRQ:451  */
    Default_Handler, /* IRQ:452  */
    Default_Handler, /* IRQ:453  */
    Default_Handler, /* IRQ:454  */
    Default_Handler, /* IRQ:455  */
    Default_Handler, /* IRQ:456  */
    Default_Handler, /* IRQ:457  */
    Default_Handler, /* IRQ:458  */
    Default_Handler, /* IRQ:459  */
    Default_Handler, /* IRQ:460  */
    Default_Handler, /* IRQ:461  */
    Default_Handler, /* IRQ:462  */
    Default_Handler, /* IRQ:463  */
    Default_Handler, /* IRQ:464  */
    Default_Handler, /* IRQ:465  */
    Default_Handler, /* IRQ:466  */
    Default_Handler, /* IRQ:467  */
    Default_Handler, /* IRQ:468  */
    Default_Handler, /* IRQ:469  */
    Default_Handler, /* IRQ:470  */
    Default_Handler, /* IRQ:471  */
    Default_Handler, /* IRQ:472  */
    Default_Handler, /* IRQ:473  */
    Default_Handler, /* IRQ:474  */
    Default_Handler, /* IRQ:475  */
    Default_Handler, /* IRQ:476  */
    Default_Handler, /* IRQ:477  */
    Default_Handler, /* IRQ:478  */
    Default_Handler, /* IRQ:479  */
};

#if defined(BSP_FEATURE_ICU_HAS_SELECT_INT)
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_MAX_ENTRIES - BSP_FEATURE_ICU_FIXED_INTSEL_COUNT] =
        {
            [0] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [1] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [2] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [3] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [4] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [5] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [6] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [7] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [8] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [9] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [10] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [11] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [12] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [13] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [14] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [15] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [16] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [17] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [18] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [19] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [20] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [21] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [22] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [23] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [24] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [25] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [26] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [27] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [28] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [29] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [30] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [31] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [32] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [33] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [34] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [35] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [36] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [37] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [38] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [39] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [40] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [41] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [42] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [43] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [44] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [45] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [46] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [47] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [48] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [49] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [50] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [51] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [52] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [53] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [54] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [55] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [56] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [57] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [58] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [59] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [60] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [61] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [62] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [63] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [64] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [65] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [66] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [67] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [68] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [69] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [70] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [71] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [72] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [73] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [74] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [75] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [76] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [77] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [78] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [79] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [80] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [81] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [82] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [83] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [84] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [85] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [86] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [87] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [88] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [89] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [90] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [91] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [92] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [93] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [94] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [95] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [96] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [97] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [98] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [99] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [100] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [101] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [102] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [103] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [104] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [105] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [106] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [107] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [108] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [109] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [110] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [111] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [112] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [113] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [114] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [115] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [116] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [117] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [118] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [119] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [120] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [121] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [122] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [123] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [124] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [125] = IRQSEL_NONE, /* No SELECT interrupt specified */
            [126] = IRQSEL_NONE, /* No SELECT interrupt specified */
        };
#endif
