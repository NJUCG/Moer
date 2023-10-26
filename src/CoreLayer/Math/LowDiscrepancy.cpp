/**
 * @file LowDiscrepancy.cpp
 * @author fengzesen (ja50n zs_feng@qq.com)
 * @brief Implemention of low-discrepancy functions.
 * @version 0.1
 * @date 2022-10-20
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 */
#include "LowDiscrepancy.h"
#include <cassert>


const int Primes[nPrimes] = {
    // clang-format off
    2, 3, 5, 7, 11,
    13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167,
    173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251,
    257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347,
    349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433,
    439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523,
    541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619,
    631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727,
    733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827,
    829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937,
    941, 947, 953, 967, 971, 977, 983, 991, 997, 1009, 1013, 1019, 1021, 1031,
    1033, 1039, 1049, 1051, 1061, 1063, 1069, 1087, 1091, 1093, 1097, 1103,
    1109, 1117, 1123, 1129, 1151, 1153, 1163, 1171, 1181, 1187, 1193, 1201,
    1213, 1217, 1223, 1229, 1231, 1237, 1249, 1259, 1277, 1279, 1283, 1289,
    1291, 1297, 1301, 1303, 1307, 1319, 1321, 1327, 1361, 1367, 1373, 1381,
    1399, 1409, 1423, 1427, 1429, 1433, 1439, 1447, 1451, 1453, 1459, 1471,
    1481, 1483, 1487, 1489, 1493, 1499, 1511, 1523, 1531, 1543, 1549, 1553,
    1559, 1567, 1571, 1579, 1583, 1597, 1601, 1607, 1609, 1613, 1619, 1621,
    1627, 1637, 1657, 1663, 1667, 1669, 1693, 1697, 1699, 1709, 1721, 1723,
    1733, 1741, 1747, 1753, 1759, 1777, 1783, 1787, 1789, 1801, 1811, 1823,
    1831, 1847, 1861, 1867, 1871, 1873, 1877, 1879, 1889, 1901, 1907, 1913,
    1931, 1933, 1949, 1951, 1973, 1979, 1987, 1993, 1997, 1999, 2003, 2011,
    2017, 2027, 2029, 2039, 2053, 2063, 2069, 2081, 2083, 2087, 2089, 2099,
    2111, 2113, 2129, 2131, 2137, 2141, 2143, 2153, 2161, 2179, 2203, 2207,
    2213, 2221, 2237, 2239, 2243, 2251, 2267, 2269, 2273, 2281, 2287, 2293,
    2297, 2309, 2311, 2333, 2339, 2341, 2347, 2351, 2357, 2371, 2377, 2381,
    2383, 2389, 2393, 2399, 2411, 2417, 2423, 2437, 2441, 2447, 2459, 2467,
    2473, 2477, 2503, 2521, 2531, 2539, 2543, 2549, 2551, 2557, 2579, 2591,
    2593, 2609, 2617, 2621, 2633, 2647, 2657, 2659, 2663, 2671, 2677, 2683,
    2687, 2689, 2693, 2699, 2707, 2711, 2713, 2719, 2729, 2731, 2741, 2749,
    2753, 2767, 2777, 2789, 2791, 2797, 2801, 2803, 2819, 2833, 2837, 2843,
    2851, 2857, 2861, 2879, 2887, 2897, 2903, 2909, 2917, 2927, 2939, 2953,
    2957, 2963, 2969, 2971, 2999, 3001, 3011, 3019, 3023, 3037, 3041, 3049,
    3061, 3067, 3079, 3083, 3089, 3109, 3119, 3121, 3137, 3163, 3167, 3169,
    3181, 3187, 3191, 3203, 3209, 3217, 3221, 3229, 3251, 3253, 3257, 3259,
    3271, 3299, 3301, 3307, 3313, 3319, 3323, 3329, 3331, 3343, 3347, 3359,
    3361, 3371, 3373, 3389, 3391, 3407, 3413, 3433, 3449, 3457, 3461, 3463,
    3467, 3469, 3491, 3499, 3511, 3517, 3527, 3529, 3533, 3539, 3541, 3547,
    3557, 3559, 3571, 3581, 3583, 3593, 3607, 3613, 3617, 3623, 3631, 3637,
    3643, 3659, 3671, 3673, 3677, 3691, 3697, 3701, 3709, 3719, 3727, 3733,
    3739, 3761, 3767, 3769, 3779, 3793, 3797, 3803, 3821, 3823, 3833, 3847,
    3851, 3853, 3863, 3877, 3881, 3889, 3907, 3911, 3917, 3919, 3923, 3929,
    3931, 3943, 3947, 3967, 3989, 4001, 4003, 4007, 4013, 4019, 4021, 4027,
    4049, 4051, 4057, 4073, 4079, 4091, 4093, 4099, 4111, 4127, 4129, 4133,
    4139, 4153, 4157, 4159, 4177, 4201, 4211, 4217, 4219, 4229, 4231, 4241,
    4243, 4253, 4259, 4261, 4271, 4273, 4283, 4289, 4297, 4327, 4337, 4339,
    4349, 4357, 4363, 4373, 4391, 4397, 4409, 4421, 4423, 4441, 4447, 4451,
    4457, 4463, 4481, 4483, 4493, 4507, 4513, 4517, 4519, 4523, 4547, 4549,
    4561, 4567, 4583, 4591, 4597, 4603, 4621, 4637, 4639, 4643, 4649, 4651,
    4657, 4663, 4673, 4679, 4691, 4703, 4721, 4723, 4729, 4733, 4751, 4759,
    4783, 4787, 4789, 4793, 4799, 4801, 4813, 4817, 4831, 4861, 4871, 4877,
    4889, 4903, 4909, 4919, 4931, 4933, 4937, 4943, 4951, 4957, 4967, 4969,
    4973, 4987, 4993, 4999, 5003, 5009, 5011, 5021, 5023, 5039, 5051, 5059,
    5077, 5081, 5087, 5099, 5101, 5107, 5113, 5119, 5147, 5153, 5167, 5171,
    5179, 5189, 5197, 5209, 5227, 5231, 5233, 5237, 5261, 5273, 5279, 5281,
    5297, 5303, 5309, 5323, 5333, 5347, 5351, 5381, 5387, 5393, 5399, 5407,
    5413, 5417, 5419, 5431, 5437, 5441, 5443, 5449, 5471, 5477, 5479, 5483,
    5501, 5503, 5507, 5519, 5521, 5527, 5531, 5557, 5563, 5569, 5573, 5581,
    5591, 5623, 5639, 5641, 5647, 5651, 5653, 5657, 5659, 5669, 5683, 5689,
    5693, 5701, 5711, 5717, 5737, 5741, 5743, 5749, 5779, 5783, 5791, 5801,
    5807, 5813, 5821, 5827, 5839, 5843, 5849, 5851, 5857, 5861, 5867, 5869,
    5879, 5881, 5897, 5903, 5923, 5927, 5939, 5953, 5981, 5987, 6007, 6011,
    6029, 6037, 6043, 6047, 6053, 6067, 6073, 6079, 6089, 6091, 6101, 6113,
    6121, 6131, 6133, 6143, 6151, 6163, 6173, 6197, 6199, 6203, 6211, 6217,
    6221, 6229, 6247, 6257, 6263, 6269, 6271, 6277, 6287, 6299, 6301, 6311,
    6317, 6323, 6329, 6337, 6343, 6353, 6359, 6361, 6367, 6373, 6379, 6389,
    6397, 6421, 6427, 6449, 6451, 6469, 6473, 6481, 6491, 6521, 6529, 6547,
    6551, 6553, 6563, 6569, 6571, 6577, 6581, 6599, 6607, 6619, 6637, 6653,
    6659, 6661, 6673, 6679, 6689, 6691, 6701, 6703, 6709, 6719, 6733, 6737,
    6761, 6763, 6779, 6781, 6791, 6793, 6803, 6823, 6827, 6829, 6833, 6841,
    6857, 6863, 6869, 6871, 6883, 6899, 6907, 6911, 6917, 6947, 6949, 6959,
    6961, 6967, 6971, 6977, 6983, 6991, 6997, 7001, 7013, 7019, 7027, 7039,
    7043, 7057, 7069, 7079, 7103, 7109, 7121, 7127, 7129, 7151, 7159, 7177,
    7187, 7193, 7207, 7211, 7213, 7219, 7229, 7237, 7243, 7247, 7253, 7283,
    7297, 7307, 7309, 7321, 7331, 7333, 7349, 7351, 7369, 7393, 7411, 7417,
    7433, 7451, 7457, 7459, 7477, 7481, 7487, 7489, 7499, 7507, 7517, 7523,
    7529, 7537, 7541, 7547, 7549, 7559, 7561, 7573, 7577, 7583, 7589, 7591,
    7603, 7607, 7621, 7639, 7643, 7649, 7669, 7673, 7681, 7687, 7691, 7699,
    7703, 7717, 7723, 7727, 7741, 7753, 7757, 7759, 7789, 7793, 7817, 7823,
    7829, 7841, 7853, 7867, 7873, 7877, 7879, 7883, 7901, 7907, 7919
    // clang-format on
};

const int PrimeSums[nPrimes] = {
    // clang-format off
    2, 5, 10, 17,
    28, 41, 58, 77, 100, 129, 160, 197, 238, 281, 328, 381, 440, 501, 568, 639,
    712, 791, 874, 963, 1060, 1161, 1264, 1371, 1480, 1593, 1720, 1851, 1988,
    2127, 2276, 2427, 2584, 2747, 2914, 3087, 3266, 3447, 3638, 3831, 4028,
    4227, 4438, 4661, 4888, 5117, 5350, 5589, 5830, 6081, 6338, 6601, 6870,
    7141, 7418, 7699, 7982, 8275, 8582, 8893, 9206, 9523, 9854, 10191, 10538,
    10887, 11240, 11599, 11966, 12339, 12718, 13101, 13490, 13887, 14288, 14697,
    15116, 15537, 15968, 16401, 16840, 17283, 17732, 18189, 18650, 19113, 19580,
    20059, 20546, 21037, 21536, 22039, 22548, 23069, 23592, 24133, 24680, 25237,
    25800, 26369, 26940, 27517, 28104, 28697, 29296, 29897, 30504, 31117, 31734,
    32353, 32984, 33625, 34268, 34915, 35568, 36227, 36888, 37561, 38238, 38921,
    39612, 40313, 41022, 41741, 42468, 43201, 43940, 44683, 45434, 46191, 46952,
    47721, 48494, 49281, 50078, 50887, 51698, 52519, 53342, 54169, 54998, 55837,
    56690, 57547, 58406, 59269, 60146, 61027, 61910, 62797, 63704, 64615, 65534,
    66463, 67400, 68341, 69288, 70241, 71208, 72179, 73156, 74139, 75130, 76127,
    77136, 78149, 79168, 80189, 81220, 82253, 83292, 84341, 85392, 86453, 87516,
    88585, 89672, 90763, 91856, 92953, 94056, 95165, 96282, 97405, 98534, 99685,
    100838, 102001, 103172, 104353, 105540, 106733, 107934, 109147, 110364,
    111587, 112816, 114047, 115284, 116533, 117792, 119069, 120348, 121631,
    122920, 124211, 125508, 126809, 128112, 129419, 130738, 132059, 133386,
    134747, 136114, 137487, 138868, 140267, 141676, 143099, 144526, 145955,
    147388, 148827, 150274, 151725, 153178, 154637, 156108, 157589, 159072,
    160559, 162048, 163541, 165040, 166551, 168074, 169605, 171148, 172697,
    174250, 175809, 177376, 178947, 180526, 182109, 183706, 185307, 186914,
    188523, 190136, 191755, 193376, 195003, 196640, 198297, 199960, 201627,
    203296, 204989, 206686, 208385, 210094, 211815, 213538, 215271, 217012,
    218759, 220512, 222271, 224048, 225831, 227618, 229407, 231208, 233019,
    234842, 236673, 238520, 240381, 242248, 244119, 245992, 247869, 249748,
    251637, 253538, 255445, 257358, 259289, 261222, 263171, 265122, 267095,
    269074, 271061, 273054, 275051, 277050, 279053, 281064, 283081, 285108,
    287137, 289176, 291229, 293292, 295361, 297442, 299525, 301612, 303701,
    305800, 307911, 310024, 312153, 314284, 316421, 318562, 320705, 322858,
    325019, 327198, 329401, 331608, 333821, 336042, 338279, 340518, 342761,
    345012, 347279, 349548, 351821, 354102, 356389, 358682, 360979, 363288,
    365599, 367932, 370271, 372612, 374959, 377310, 379667, 382038, 384415,
    386796, 389179, 391568, 393961, 396360, 398771, 401188, 403611, 406048,
    408489, 410936, 413395, 415862, 418335, 420812, 423315, 425836, 428367,
    430906, 433449, 435998, 438549, 441106, 443685, 446276, 448869, 451478,
    454095, 456716, 459349, 461996, 464653, 467312, 469975, 472646, 475323,
    478006, 480693, 483382, 486075, 488774, 491481, 494192, 496905, 499624,
    502353, 505084, 507825, 510574, 513327, 516094, 518871, 521660, 524451,
    527248, 530049, 532852, 535671, 538504, 541341, 544184, 547035, 549892,
    552753, 555632, 558519, 561416, 564319, 567228, 570145, 573072, 576011,
    578964, 581921, 584884, 587853, 590824, 593823, 596824, 599835, 602854,
    605877, 608914, 611955, 615004, 618065, 621132, 624211, 627294, 630383,
    633492, 636611, 639732, 642869, 646032, 649199, 652368, 655549, 658736,
    661927, 665130, 668339, 671556, 674777, 678006, 681257, 684510, 687767,
    691026, 694297, 697596, 700897, 704204, 707517, 710836, 714159, 717488,
    720819, 724162, 727509, 730868, 734229, 737600, 740973, 744362, 747753,
    751160, 754573, 758006, 761455, 764912, 768373, 771836, 775303, 778772,
    782263, 785762, 789273, 792790, 796317, 799846, 803379, 806918, 810459,
    814006, 817563, 821122, 824693, 828274, 831857, 835450, 839057, 842670,
    846287, 849910, 853541, 857178, 860821, 864480, 868151, 871824, 875501,
    879192, 882889, 886590, 890299, 894018, 897745, 901478, 905217, 908978,
    912745, 916514, 920293, 924086, 927883, 931686, 935507, 939330, 943163,
    947010, 950861, 954714, 958577, 962454, 966335, 970224, 974131, 978042,
    981959, 985878, 989801, 993730, 997661, 1001604, 1005551, 1009518, 1013507,
    1017508, 1021511, 1025518, 1029531, 1033550, 1037571, 1041598, 1045647,
    1049698, 1053755, 1057828, 1061907, 1065998, 1070091, 1074190, 1078301,
    1082428, 1086557, 1090690, 1094829, 1098982, 1103139, 1107298, 1111475,
    1115676, 1119887, 1124104, 1128323, 1132552, 1136783, 1141024, 1145267,
    1149520, 1153779, 1158040, 1162311, 1166584, 1170867, 1175156, 1179453,
    1183780, 1188117, 1192456, 1196805, 1201162, 1205525, 1209898, 1214289,
    1218686, 1223095, 1227516, 1231939, 1236380, 1240827, 1245278, 1249735,
    1254198, 1258679, 1263162, 1267655, 1272162, 1276675, 1281192, 1285711,
    1290234, 1294781, 1299330, 1303891, 1308458, 1313041, 1317632, 1322229,
    1326832, 1331453, 1336090, 1340729, 1345372, 1350021, 1354672, 1359329,
    1363992, 1368665, 1373344, 1378035, 1382738, 1387459, 1392182, 1396911,
    1401644, 1406395, 1411154, 1415937, 1420724, 1425513, 1430306, 1435105,
    1439906, 1444719, 1449536, 1454367, 1459228, 1464099, 1468976, 1473865,
    1478768, 1483677, 1488596, 1493527, 1498460, 1503397, 1508340, 1513291,
    1518248, 1523215, 1528184, 1533157, 1538144, 1543137, 1548136, 1553139,
    1558148, 1563159, 1568180, 1573203, 1578242, 1583293, 1588352, 1593429,
    1598510, 1603597, 1608696, 1613797, 1618904, 1624017, 1629136, 1634283,
    1639436, 1644603, 1649774, 1654953, 1660142, 1665339, 1670548, 1675775,
    1681006, 1686239, 1691476, 1696737, 1702010, 1707289, 1712570, 1717867,
    1723170, 1728479, 1733802, 1739135, 1744482, 1749833, 1755214, 1760601,
    1765994, 1771393, 1776800, 1782213, 1787630, 1793049, 1798480, 1803917,
    1809358, 1814801, 1820250, 1825721, 1831198, 1836677, 1842160, 1847661,
    1853164, 1858671, 1864190, 1869711, 1875238, 1880769, 1886326, 1891889,
    1897458, 1903031, 1908612, 1914203, 1919826, 1925465, 1931106, 1936753,
    1942404, 1948057, 1953714, 1959373, 1965042, 1970725, 1976414, 1982107,
    1987808, 1993519, 1999236, 2004973, 2010714, 2016457, 2022206, 2027985,
    2033768, 2039559, 2045360, 2051167, 2056980, 2062801, 2068628, 2074467,
    2080310, 2086159, 2092010, 2097867, 2103728, 2109595, 2115464, 2121343,
    2127224, 2133121, 2139024, 2144947, 2150874, 2156813, 2162766, 2168747,
    2174734, 2180741, 2186752, 2192781, 2198818, 2204861, 2210908, 2216961,
    2223028, 2229101, 2235180, 2241269, 2247360, 2253461, 2259574, 2265695,
    2271826, 2277959, 2284102, 2290253, 2296416, 2302589, 2308786, 2314985,
    2321188, 2327399, 2333616, 2339837, 2346066, 2352313, 2358570, 2364833,
    2371102, 2377373, 2383650, 2389937, 2396236, 2402537, 2408848, 2415165,
    2421488, 2427817, 2434154, 2440497, 2446850, 2453209, 2459570, 2465937,
    2472310, 2478689, 2485078, 2491475, 2497896, 2504323, 2510772, 2517223,
    2523692, 2530165, 2536646, 2543137, 2549658, 2556187, 2562734, 2569285,
    2575838, 2582401, 2588970, 2595541, 2602118, 2608699, 2615298, 2621905,
    2628524, 2635161, 2641814, 2648473, 2655134, 2661807, 2668486, 2675175,
    2681866, 2688567, 2695270, 2701979, 2708698, 2715431, 2722168, 2728929,
    2735692, 2742471, 2749252, 2756043, 2762836, 2769639, 2776462, 2783289,
    2790118, 2796951, 2803792, 2810649, 2817512, 2824381, 2831252, 2838135,
    2845034, 2851941, 2858852, 2865769, 2872716, 2879665, 2886624, 2893585,
    2900552, 2907523, 2914500, 2921483, 2928474, 2935471, 2942472, 2949485,
    2956504, 2963531, 2970570, 2977613, 2984670, 2991739, 2998818, 3005921,
    3013030, 3020151, 3027278, 3034407, 3041558, 3048717, 3055894, 3063081,
    3070274, 3077481, 3084692, 3091905, 3099124, 3106353, 3113590, 3120833,
    3128080, 3135333, 3142616, 3149913, 3157220, 3164529, 3171850, 3179181,
    3186514, 3193863, 3201214, 3208583, 3215976, 3223387, 3230804, 3238237,
    3245688, 3253145, 3260604, 3268081, 3275562, 3283049, 3290538, 3298037,
    3305544, 3313061, 3320584, 3328113, 3335650, 3343191, 3350738, 3358287,
    3365846, 3373407, 3380980, 3388557, 3396140, 3403729, 3411320, 3418923,
    3426530, 3434151, 3441790, 3449433, 3457082, 3464751, 3472424, 3480105,
    3487792, 3495483, 3503182, 3510885, 3518602, 3526325, 3534052, 3541793,
    3549546, 3557303, 3565062, 3572851, 3580644, 3588461, 3596284, 3604113,
    3611954, 3619807, 3627674, 3635547, 3643424, 3651303, 3659186, 3667087,
    3674994, 3682913
    // clang-format on
};

// Static functions for common radical inverse with or without scramble
template<int base>
static double radicalInverseLoop(uint64_t a) {
    constexpr double invBase = 1.0 / base;
    uint64_t reverseDigits = 0;
    double invBaseN = 1;
    while (a) {
        uint64_t nextA = a / base;
        uint64_t digit = a - nextA * base;
        reverseDigits = reverseDigits * base + digit;
        invBaseN *= invBase;
        a = nextA;
    }
    return std::min(reverseDigits * invBaseN, ONEMINUSEPSILON);
}
template<int base>
static double radicalInverseScrambleLoop(const uint16_t *perm, uint64_t a) {
    constexpr double invBase = 1.0 / base;
    uint64_t reverseDigits = 0;
    double invBaseN = 1;
    while (a) {
        uint64_t nextA = a / base;
        uint64_t digit = a - nextA * base;
        reverseDigits = reverseDigits * base + perm[digit];
        invBaseN *= invBase;
        a = nextA;
    }
    // The trailing zeros are also required to be replaced by perm[0].
    return std::min(
        invBaseN * (reverseDigits + invBase * perm[0] / (1 - invBase)),
        ONEMINUSEPSILON);
}
/**
 * @brief To shuffle vectors in a continuous memory.
 *
 * @tparam T      Value type.
 * @param vecs  Pointer to the first value of first vector.
 * @param count   Number of vectors.
 * @param nDim    Number of dimensions.
 * @param rng     Random number generator.
 */
template<typename T>
static void shuffleByPointer(T *vecs, int count, int nDim,
                             RandomNumberGenerator &rng) {
    for (int i = 0; i < count; i++) {
        int other = rng(i, count);
        for (int j = 0; j < nDim; j++)
            std::swap(vecs[i * nDim + j], vecs[other * nDim + j]);
    }
}
std::vector<uint16_t> computeScramblePermutations(RandomNumberGenerator &rng) {
    std::vector<uint16_t> perms;
    // Count the total size
    int arrSize = PrimeSums[nPrimes - 1];
    // for (int i = 0; i < nPrimes; i++) arrSize += Primes[i];
    perms.resize(arrSize);
    // Generate permutations
    uint16_t *p = &perms[0];// Used to travel the vector
    for (int i = 0; i < nPrimes; i++) {
        // Permutation for the i-th prime
        // Init
        for (int j = 0; j < Primes[i]; j++) p[j] = j;
        // Shuffle
        shuffleByPointer(p, Primes[i], 1, rng);
        p += Primes[i];
    }
    return perms;
}
double radicalInverse(int baseIndex, uint64_t a) {
    assert(0 <= baseIndex && baseIndex < 1024);
    switch (baseIndex) {
        // Use machine bits to compute base-2 radical inverse
        case 0:
            return reverseBits64(a) * 0x1p-64;
        case 1:
            return radicalInverseLoop<3>(a);
        case 2:
            return radicalInverseLoop<5>(a);
        case 3:
            return radicalInverseLoop<7>(a);
        case 4:
            return radicalInverseLoop<11>(a);
        case 5:
            return radicalInverseLoop<13>(a);
        case 6:
            return radicalInverseLoop<17>(a);
        case 7:
            return radicalInverseLoop<19>(a);
        case 8:
            return radicalInverseLoop<23>(a);
        case 9:
            return radicalInverseLoop<29>(a);
        case 10:
            return radicalInverseLoop<31>(a);
        case 11:
            return radicalInverseLoop<37>(a);
        case 12:
            return radicalInverseLoop<41>(a);
        case 13:
            return radicalInverseLoop<43>(a);
        case 14:
            return radicalInverseLoop<47>(a);
        case 15:
            return radicalInverseLoop<53>(a);
        case 16:
            return radicalInverseLoop<59>(a);
        case 17:
            return radicalInverseLoop<61>(a);
        case 18:
            return radicalInverseLoop<67>(a);
        case 19:
            return radicalInverseLoop<71>(a);
        case 20:
            return radicalInverseLoop<73>(a);
        case 21:
            return radicalInverseLoop<79>(a);
        case 22:
            return radicalInverseLoop<83>(a);
        case 23:
            return radicalInverseLoop<89>(a);
        case 24:
            return radicalInverseLoop<97>(a);
        case 25:
            return radicalInverseLoop<101>(a);
        case 26:
            return radicalInverseLoop<103>(a);
        case 27:
            return radicalInverseLoop<107>(a);
        case 28:
            return radicalInverseLoop<109>(a);
        case 29:
            return radicalInverseLoop<113>(a);
        case 30:
            return radicalInverseLoop<127>(a);
        case 31:
            return radicalInverseLoop<131>(a);
        case 32:
            return radicalInverseLoop<137>(a);
        case 33:
            return radicalInverseLoop<139>(a);
        case 34:
            return radicalInverseLoop<149>(a);
        case 35:
            return radicalInverseLoop<151>(a);
        case 36:
            return radicalInverseLoop<157>(a);
        case 37:
            return radicalInverseLoop<163>(a);
        case 38:
            return radicalInverseLoop<167>(a);
        case 39:
            return radicalInverseLoop<173>(a);
        case 40:
            return radicalInverseLoop<179>(a);
        case 41:
            return radicalInverseLoop<181>(a);
        case 42:
            return radicalInverseLoop<191>(a);
        case 43:
            return radicalInverseLoop<193>(a);
        case 44:
            return radicalInverseLoop<197>(a);
        case 45:
            return radicalInverseLoop<199>(a);
        case 46:
            return radicalInverseLoop<211>(a);
        case 47:
            return radicalInverseLoop<223>(a);
        case 48:
            return radicalInverseLoop<227>(a);
        case 49:
            return radicalInverseLoop<229>(a);
        case 50:
            return radicalInverseLoop<233>(a);
        case 51:
            return radicalInverseLoop<239>(a);
        case 52:
            return radicalInverseLoop<241>(a);
        case 53:
            return radicalInverseLoop<251>(a);
        case 54:
            return radicalInverseLoop<257>(a);
        case 55:
            return radicalInverseLoop<263>(a);
        case 56:
            return radicalInverseLoop<269>(a);
        case 57:
            return radicalInverseLoop<271>(a);
        case 58:
            return radicalInverseLoop<277>(a);
        case 59:
            return radicalInverseLoop<281>(a);
        case 60:
            return radicalInverseLoop<283>(a);
        case 61:
            return radicalInverseLoop<293>(a);
        case 62:
            return radicalInverseLoop<307>(a);
        case 63:
            return radicalInverseLoop<311>(a);
        case 64:
            return radicalInverseLoop<313>(a);
        case 65:
            return radicalInverseLoop<317>(a);
        case 66:
            return radicalInverseLoop<331>(a);
        case 67:
            return radicalInverseLoop<337>(a);
        case 68:
            return radicalInverseLoop<347>(a);
        case 69:
            return radicalInverseLoop<349>(a);
        case 70:
            return radicalInverseLoop<353>(a);
        case 71:
            return radicalInverseLoop<359>(a);
        case 72:
            return radicalInverseLoop<367>(a);
        case 73:
            return radicalInverseLoop<373>(a);
        case 74:
            return radicalInverseLoop<379>(a);
        case 75:
            return radicalInverseLoop<383>(a);
        case 76:
            return radicalInverseLoop<389>(a);
        case 77:
            return radicalInverseLoop<397>(a);
        case 78:
            return radicalInverseLoop<401>(a);
        case 79:
            return radicalInverseLoop<409>(a);
        case 80:
            return radicalInverseLoop<419>(a);
        case 81:
            return radicalInverseLoop<421>(a);
        case 82:
            return radicalInverseLoop<431>(a);
        case 83:
            return radicalInverseLoop<433>(a);
        case 84:
            return radicalInverseLoop<439>(a);
        case 85:
            return radicalInverseLoop<443>(a);
        case 86:
            return radicalInverseLoop<449>(a);
        case 87:
            return radicalInverseLoop<457>(a);
        case 88:
            return radicalInverseLoop<461>(a);
        case 89:
            return radicalInverseLoop<463>(a);
        case 90:
            return radicalInverseLoop<467>(a);
        case 91:
            return radicalInverseLoop<479>(a);
        case 92:
            return radicalInverseLoop<487>(a);
        case 93:
            return radicalInverseLoop<491>(a);
        case 94:
            return radicalInverseLoop<499>(a);
        case 95:
            return radicalInverseLoop<503>(a);
        case 96:
            return radicalInverseLoop<509>(a);
        case 97:
            return radicalInverseLoop<521>(a);
        case 98:
            return radicalInverseLoop<523>(a);
        case 99:
            return radicalInverseLoop<541>(a);
        case 100:
            return radicalInverseLoop<547>(a);
        case 101:
            return radicalInverseLoop<557>(a);
        case 102:
            return radicalInverseLoop<563>(a);
        case 103:
            return radicalInverseLoop<569>(a);
        case 104:
            return radicalInverseLoop<571>(a);
        case 105:
            return radicalInverseLoop<577>(a);
        case 106:
            return radicalInverseLoop<587>(a);
        case 107:
            return radicalInverseLoop<593>(a);
        case 108:
            return radicalInverseLoop<599>(a);
        case 109:
            return radicalInverseLoop<601>(a);
        case 110:
            return radicalInverseLoop<607>(a);
        case 111:
            return radicalInverseLoop<613>(a);
        case 112:
            return radicalInverseLoop<617>(a);
        case 113:
            return radicalInverseLoop<619>(a);
        case 114:
            return radicalInverseLoop<631>(a);
        case 115:
            return radicalInverseLoop<641>(a);
        case 116:
            return radicalInverseLoop<643>(a);
        case 117:
            return radicalInverseLoop<647>(a);
        case 118:
            return radicalInverseLoop<653>(a);
        case 119:
            return radicalInverseLoop<659>(a);
        case 120:
            return radicalInverseLoop<661>(a);
        case 121:
            return radicalInverseLoop<673>(a);
        case 122:
            return radicalInverseLoop<677>(a);
        case 123:
            return radicalInverseLoop<683>(a);
        case 124:
            return radicalInverseLoop<691>(a);
        case 125:
            return radicalInverseLoop<701>(a);
        case 126:
            return radicalInverseLoop<709>(a);
        case 127:
            return radicalInverseLoop<719>(a);
        case 128:
            return radicalInverseLoop<727>(a);
        case 129:
            return radicalInverseLoop<733>(a);
        case 130:
            return radicalInverseLoop<739>(a);
        case 131:
            return radicalInverseLoop<743>(a);
        case 132:
            return radicalInverseLoop<751>(a);
        case 133:
            return radicalInverseLoop<757>(a);
        case 134:
            return radicalInverseLoop<761>(a);
        case 135:
            return radicalInverseLoop<769>(a);
        case 136:
            return radicalInverseLoop<773>(a);
        case 137:
            return radicalInverseLoop<787>(a);
        case 138:
            return radicalInverseLoop<797>(a);
        case 139:
            return radicalInverseLoop<809>(a);
        case 140:
            return radicalInverseLoop<811>(a);
        case 141:
            return radicalInverseLoop<821>(a);
        case 142:
            return radicalInverseLoop<823>(a);
        case 143:
            return radicalInverseLoop<827>(a);
        case 144:
            return radicalInverseLoop<829>(a);
        case 145:
            return radicalInverseLoop<839>(a);
        case 146:
            return radicalInverseLoop<853>(a);
        case 147:
            return radicalInverseLoop<857>(a);
        case 148:
            return radicalInverseLoop<859>(a);
        case 149:
            return radicalInverseLoop<863>(a);
        case 150:
            return radicalInverseLoop<877>(a);
        case 151:
            return radicalInverseLoop<881>(a);
        case 152:
            return radicalInverseLoop<883>(a);
        case 153:
            return radicalInverseLoop<887>(a);
        case 154:
            return radicalInverseLoop<907>(a);
        case 155:
            return radicalInverseLoop<911>(a);
        case 156:
            return radicalInverseLoop<919>(a);
        case 157:
            return radicalInverseLoop<929>(a);
        case 158:
            return radicalInverseLoop<937>(a);
        case 159:
            return radicalInverseLoop<941>(a);
        case 160:
            return radicalInverseLoop<947>(a);
        case 161:
            return radicalInverseLoop<953>(a);
        case 162:
            return radicalInverseLoop<967>(a);
        case 163:
            return radicalInverseLoop<971>(a);
        case 164:
            return radicalInverseLoop<977>(a);
        case 165:
            return radicalInverseLoop<983>(a);
        case 166:
            return radicalInverseLoop<991>(a);
        case 167:
            return radicalInverseLoop<997>(a);
        case 168:
            return radicalInverseLoop<1009>(a);
        case 169:
            return radicalInverseLoop<1013>(a);
        case 170:
            return radicalInverseLoop<1019>(a);
        case 171:
            return radicalInverseLoop<1021>(a);
        case 172:
            return radicalInverseLoop<1031>(a);
        case 173:
            return radicalInverseLoop<1033>(a);
        case 174:
            return radicalInverseLoop<1039>(a);
        case 175:
            return radicalInverseLoop<1049>(a);
        case 176:
            return radicalInverseLoop<1051>(a);
        case 177:
            return radicalInverseLoop<1061>(a);
        case 178:
            return radicalInverseLoop<1063>(a);
        case 179:
            return radicalInverseLoop<1069>(a);
        case 180:
            return radicalInverseLoop<1087>(a);
        case 181:
            return radicalInverseLoop<1091>(a);
        case 182:
            return radicalInverseLoop<1093>(a);
        case 183:
            return radicalInverseLoop<1097>(a);
        case 184:
            return radicalInverseLoop<1103>(a);
        case 185:
            return radicalInverseLoop<1109>(a);
        case 186:
            return radicalInverseLoop<1117>(a);
        case 187:
            return radicalInverseLoop<1123>(a);
        case 188:
            return radicalInverseLoop<1129>(a);
        case 189:
            return radicalInverseLoop<1151>(a);
        case 190:
            return radicalInverseLoop<1153>(a);
        case 191:
            return radicalInverseLoop<1163>(a);
        case 192:
            return radicalInverseLoop<1171>(a);
        case 193:
            return radicalInverseLoop<1181>(a);
        case 194:
            return radicalInverseLoop<1187>(a);
        case 195:
            return radicalInverseLoop<1193>(a);
        case 196:
            return radicalInverseLoop<1201>(a);
        case 197:
            return radicalInverseLoop<1213>(a);
        case 198:
            return radicalInverseLoop<1217>(a);
        case 199:
            return radicalInverseLoop<1223>(a);
        case 200:
            return radicalInverseLoop<1229>(a);
        case 201:
            return radicalInverseLoop<1231>(a);
        case 202:
            return radicalInverseLoop<1237>(a);
        case 203:
            return radicalInverseLoop<1249>(a);
        case 204:
            return radicalInverseLoop<1259>(a);
        case 205:
            return radicalInverseLoop<1277>(a);
        case 206:
            return radicalInverseLoop<1279>(a);
        case 207:
            return radicalInverseLoop<1283>(a);
        case 208:
            return radicalInverseLoop<1289>(a);
        case 209:
            return radicalInverseLoop<1291>(a);
        case 210:
            return radicalInverseLoop<1297>(a);
        case 211:
            return radicalInverseLoop<1301>(a);
        case 212:
            return radicalInverseLoop<1303>(a);
        case 213:
            return radicalInverseLoop<1307>(a);
        case 214:
            return radicalInverseLoop<1319>(a);
        case 215:
            return radicalInverseLoop<1321>(a);
        case 216:
            return radicalInverseLoop<1327>(a);
        case 217:
            return radicalInverseLoop<1361>(a);
        case 218:
            return radicalInverseLoop<1367>(a);
        case 219:
            return radicalInverseLoop<1373>(a);
        case 220:
            return radicalInverseLoop<1381>(a);
        case 221:
            return radicalInverseLoop<1399>(a);
        case 222:
            return radicalInverseLoop<1409>(a);
        case 223:
            return radicalInverseLoop<1423>(a);
        case 224:
            return radicalInverseLoop<1427>(a);
        case 225:
            return radicalInverseLoop<1429>(a);
        case 226:
            return radicalInverseLoop<1433>(a);
        case 227:
            return radicalInverseLoop<1439>(a);
        case 228:
            return radicalInverseLoop<1447>(a);
        case 229:
            return radicalInverseLoop<1451>(a);
        case 230:
            return radicalInverseLoop<1453>(a);
        case 231:
            return radicalInverseLoop<1459>(a);
        case 232:
            return radicalInverseLoop<1471>(a);
        case 233:
            return radicalInverseLoop<1481>(a);
        case 234:
            return radicalInverseLoop<1483>(a);
        case 235:
            return radicalInverseLoop<1487>(a);
        case 236:
            return radicalInverseLoop<1489>(a);
        case 237:
            return radicalInverseLoop<1493>(a);
        case 238:
            return radicalInverseLoop<1499>(a);
        case 239:
            return radicalInverseLoop<1511>(a);
        case 240:
            return radicalInverseLoop<1523>(a);
        case 241:
            return radicalInverseLoop<1531>(a);
        case 242:
            return radicalInverseLoop<1543>(a);
        case 243:
            return radicalInverseLoop<1549>(a);
        case 244:
            return radicalInverseLoop<1553>(a);
        case 245:
            return radicalInverseLoop<1559>(a);
        case 246:
            return radicalInverseLoop<1567>(a);
        case 247:
            return radicalInverseLoop<1571>(a);
        case 248:
            return radicalInverseLoop<1579>(a);
        case 249:
            return radicalInverseLoop<1583>(a);
        case 250:
            return radicalInverseLoop<1597>(a);
        case 251:
            return radicalInverseLoop<1601>(a);
        case 252:
            return radicalInverseLoop<1607>(a);
        case 253:
            return radicalInverseLoop<1609>(a);
        case 254:
            return radicalInverseLoop<1613>(a);
        case 255:
            return radicalInverseLoop<1619>(a);
        case 256:
            return radicalInverseLoop<1621>(a);
        case 257:
            return radicalInverseLoop<1627>(a);
        case 258:
            return radicalInverseLoop<1637>(a);
        case 259:
            return radicalInverseLoop<1657>(a);
        case 260:
            return radicalInverseLoop<1663>(a);
        case 261:
            return radicalInverseLoop<1667>(a);
        case 262:
            return radicalInverseLoop<1669>(a);
        case 263:
            return radicalInverseLoop<1693>(a);
        case 264:
            return radicalInverseLoop<1697>(a);
        case 265:
            return radicalInverseLoop<1699>(a);
        case 266:
            return radicalInverseLoop<1709>(a);
        case 267:
            return radicalInverseLoop<1721>(a);
        case 268:
            return radicalInverseLoop<1723>(a);
        case 269:
            return radicalInverseLoop<1733>(a);
        case 270:
            return radicalInverseLoop<1741>(a);
        case 271:
            return radicalInverseLoop<1747>(a);
        case 272:
            return radicalInverseLoop<1753>(a);
        case 273:
            return radicalInverseLoop<1759>(a);
        case 274:
            return radicalInverseLoop<1777>(a);
        case 275:
            return radicalInverseLoop<1783>(a);
        case 276:
            return radicalInverseLoop<1787>(a);
        case 277:
            return radicalInverseLoop<1789>(a);
        case 278:
            return radicalInverseLoop<1801>(a);
        case 279:
            return radicalInverseLoop<1811>(a);
        case 280:
            return radicalInverseLoop<1823>(a);
        case 281:
            return radicalInverseLoop<1831>(a);
        case 282:
            return radicalInverseLoop<1847>(a);
        case 283:
            return radicalInverseLoop<1861>(a);
        case 284:
            return radicalInverseLoop<1867>(a);
        case 285:
            return radicalInverseLoop<1871>(a);
        case 286:
            return radicalInverseLoop<1873>(a);
        case 287:
            return radicalInverseLoop<1877>(a);
        case 288:
            return radicalInverseLoop<1879>(a);
        case 289:
            return radicalInverseLoop<1889>(a);
        case 290:
            return radicalInverseLoop<1901>(a);
        case 291:
            return radicalInverseLoop<1907>(a);
        case 292:
            return radicalInverseLoop<1913>(a);
        case 293:
            return radicalInverseLoop<1931>(a);
        case 294:
            return radicalInverseLoop<1933>(a);
        case 295:
            return radicalInverseLoop<1949>(a);
        case 296:
            return radicalInverseLoop<1951>(a);
        case 297:
            return radicalInverseLoop<1973>(a);
        case 298:
            return radicalInverseLoop<1979>(a);
        case 299:
            return radicalInverseLoop<1987>(a);
        case 300:
            return radicalInverseLoop<1993>(a);
        case 301:
            return radicalInverseLoop<1997>(a);
        case 302:
            return radicalInverseLoop<1999>(a);
        case 303:
            return radicalInverseLoop<2003>(a);
        case 304:
            return radicalInverseLoop<2011>(a);
        case 305:
            return radicalInverseLoop<2017>(a);
        case 306:
            return radicalInverseLoop<2027>(a);
        case 307:
            return radicalInverseLoop<2029>(a);
        case 308:
            return radicalInverseLoop<2039>(a);
        case 309:
            return radicalInverseLoop<2053>(a);
        case 310:
            return radicalInverseLoop<2063>(a);
        case 311:
            return radicalInverseLoop<2069>(a);
        case 312:
            return radicalInverseLoop<2081>(a);
        case 313:
            return radicalInverseLoop<2083>(a);
        case 314:
            return radicalInverseLoop<2087>(a);
        case 315:
            return radicalInverseLoop<2089>(a);
        case 316:
            return radicalInverseLoop<2099>(a);
        case 317:
            return radicalInverseLoop<2111>(a);
        case 318:
            return radicalInverseLoop<2113>(a);
        case 319:
            return radicalInverseLoop<2129>(a);
        case 320:
            return radicalInverseLoop<2131>(a);
        case 321:
            return radicalInverseLoop<2137>(a);
        case 322:
            return radicalInverseLoop<2141>(a);
        case 323:
            return radicalInverseLoop<2143>(a);
        case 324:
            return radicalInverseLoop<2153>(a);
        case 325:
            return radicalInverseLoop<2161>(a);
        case 326:
            return radicalInverseLoop<2179>(a);
        case 327:
            return radicalInverseLoop<2203>(a);
        case 328:
            return radicalInverseLoop<2207>(a);
        case 329:
            return radicalInverseLoop<2213>(a);
        case 330:
            return radicalInverseLoop<2221>(a);
        case 331:
            return radicalInverseLoop<2237>(a);
        case 332:
            return radicalInverseLoop<2239>(a);
        case 333:
            return radicalInverseLoop<2243>(a);
        case 334:
            return radicalInverseLoop<2251>(a);
        case 335:
            return radicalInverseLoop<2267>(a);
        case 336:
            return radicalInverseLoop<2269>(a);
        case 337:
            return radicalInverseLoop<2273>(a);
        case 338:
            return radicalInverseLoop<2281>(a);
        case 339:
            return radicalInverseLoop<2287>(a);
        case 340:
            return radicalInverseLoop<2293>(a);
        case 341:
            return radicalInverseLoop<2297>(a);
        case 342:
            return radicalInverseLoop<2309>(a);
        case 343:
            return radicalInverseLoop<2311>(a);
        case 344:
            return radicalInverseLoop<2333>(a);
        case 345:
            return radicalInverseLoop<2339>(a);
        case 346:
            return radicalInverseLoop<2341>(a);
        case 347:
            return radicalInverseLoop<2347>(a);
        case 348:
            return radicalInverseLoop<2351>(a);
        case 349:
            return radicalInverseLoop<2357>(a);
        case 350:
            return radicalInverseLoop<2371>(a);
        case 351:
            return radicalInverseLoop<2377>(a);
        case 352:
            return radicalInverseLoop<2381>(a);
        case 353:
            return radicalInverseLoop<2383>(a);
        case 354:
            return radicalInverseLoop<2389>(a);
        case 355:
            return radicalInverseLoop<2393>(a);
        case 356:
            return radicalInverseLoop<2399>(a);
        case 357:
            return radicalInverseLoop<2411>(a);
        case 358:
            return radicalInverseLoop<2417>(a);
        case 359:
            return radicalInverseLoop<2423>(a);
        case 360:
            return radicalInverseLoop<2437>(a);
        case 361:
            return radicalInverseLoop<2441>(a);
        case 362:
            return radicalInverseLoop<2447>(a);
        case 363:
            return radicalInverseLoop<2459>(a);
        case 364:
            return radicalInverseLoop<2467>(a);
        case 365:
            return radicalInverseLoop<2473>(a);
        case 366:
            return radicalInverseLoop<2477>(a);
        case 367:
            return radicalInverseLoop<2503>(a);
        case 368:
            return radicalInverseLoop<2521>(a);
        case 369:
            return radicalInverseLoop<2531>(a);
        case 370:
            return radicalInverseLoop<2539>(a);
        case 371:
            return radicalInverseLoop<2543>(a);
        case 372:
            return radicalInverseLoop<2549>(a);
        case 373:
            return radicalInverseLoop<2551>(a);
        case 374:
            return radicalInverseLoop<2557>(a);
        case 375:
            return radicalInverseLoop<2579>(a);
        case 376:
            return radicalInverseLoop<2591>(a);
        case 377:
            return radicalInverseLoop<2593>(a);
        case 378:
            return radicalInverseLoop<2609>(a);
        case 379:
            return radicalInverseLoop<2617>(a);
        case 380:
            return radicalInverseLoop<2621>(a);
        case 381:
            return radicalInverseLoop<2633>(a);
        case 382:
            return radicalInverseLoop<2647>(a);
        case 383:
            return radicalInverseLoop<2657>(a);
        case 384:
            return radicalInverseLoop<2659>(a);
        case 385:
            return radicalInverseLoop<2663>(a);
        case 386:
            return radicalInverseLoop<2671>(a);
        case 387:
            return radicalInverseLoop<2677>(a);
        case 388:
            return radicalInverseLoop<2683>(a);
        case 389:
            return radicalInverseLoop<2687>(a);
        case 390:
            return radicalInverseLoop<2689>(a);
        case 391:
            return radicalInverseLoop<2693>(a);
        case 392:
            return radicalInverseLoop<2699>(a);
        case 393:
            return radicalInverseLoop<2707>(a);
        case 394:
            return radicalInverseLoop<2711>(a);
        case 395:
            return radicalInverseLoop<2713>(a);
        case 396:
            return radicalInverseLoop<2719>(a);
        case 397:
            return radicalInverseLoop<2729>(a);
        case 398:
            return radicalInverseLoop<2731>(a);
        case 399:
            return radicalInverseLoop<2741>(a);
        case 400:
            return radicalInverseLoop<2749>(a);
        case 401:
            return radicalInverseLoop<2753>(a);
        case 402:
            return radicalInverseLoop<2767>(a);
        case 403:
            return radicalInverseLoop<2777>(a);
        case 404:
            return radicalInverseLoop<2789>(a);
        case 405:
            return radicalInverseLoop<2791>(a);
        case 406:
            return radicalInverseLoop<2797>(a);
        case 407:
            return radicalInverseLoop<2801>(a);
        case 408:
            return radicalInverseLoop<2803>(a);
        case 409:
            return radicalInverseLoop<2819>(a);
        case 410:
            return radicalInverseLoop<2833>(a);
        case 411:
            return radicalInverseLoop<2837>(a);
        case 412:
            return radicalInverseLoop<2843>(a);
        case 413:
            return radicalInverseLoop<2851>(a);
        case 414:
            return radicalInverseLoop<2857>(a);
        case 415:
            return radicalInverseLoop<2861>(a);
        case 416:
            return radicalInverseLoop<2879>(a);
        case 417:
            return radicalInverseLoop<2887>(a);
        case 418:
            return radicalInverseLoop<2897>(a);
        case 419:
            return radicalInverseLoop<2903>(a);
        case 420:
            return radicalInverseLoop<2909>(a);
        case 421:
            return radicalInverseLoop<2917>(a);
        case 422:
            return radicalInverseLoop<2927>(a);
        case 423:
            return radicalInverseLoop<2939>(a);
        case 424:
            return radicalInverseLoop<2953>(a);
        case 425:
            return radicalInverseLoop<2957>(a);
        case 426:
            return radicalInverseLoop<2963>(a);
        case 427:
            return radicalInverseLoop<2969>(a);
        case 428:
            return radicalInverseLoop<2971>(a);
        case 429:
            return radicalInverseLoop<2999>(a);
        case 430:
            return radicalInverseLoop<3001>(a);
        case 431:
            return radicalInverseLoop<3011>(a);
        case 432:
            return radicalInverseLoop<3019>(a);
        case 433:
            return radicalInverseLoop<3023>(a);
        case 434:
            return radicalInverseLoop<3037>(a);
        case 435:
            return radicalInverseLoop<3041>(a);
        case 436:
            return radicalInverseLoop<3049>(a);
        case 437:
            return radicalInverseLoop<3061>(a);
        case 438:
            return radicalInverseLoop<3067>(a);
        case 439:
            return radicalInverseLoop<3079>(a);
        case 440:
            return radicalInverseLoop<3083>(a);
        case 441:
            return radicalInverseLoop<3089>(a);
        case 442:
            return radicalInverseLoop<3109>(a);
        case 443:
            return radicalInverseLoop<3119>(a);
        case 444:
            return radicalInverseLoop<3121>(a);
        case 445:
            return radicalInverseLoop<3137>(a);
        case 446:
            return radicalInverseLoop<3163>(a);
        case 447:
            return radicalInverseLoop<3167>(a);
        case 448:
            return radicalInverseLoop<3169>(a);
        case 449:
            return radicalInverseLoop<3181>(a);
        case 450:
            return radicalInverseLoop<3187>(a);
        case 451:
            return radicalInverseLoop<3191>(a);
        case 452:
            return radicalInverseLoop<3203>(a);
        case 453:
            return radicalInverseLoop<3209>(a);
        case 454:
            return radicalInverseLoop<3217>(a);
        case 455:
            return radicalInverseLoop<3221>(a);
        case 456:
            return radicalInverseLoop<3229>(a);
        case 457:
            return radicalInverseLoop<3251>(a);
        case 458:
            return radicalInverseLoop<3253>(a);
        case 459:
            return radicalInverseLoop<3257>(a);
        case 460:
            return radicalInverseLoop<3259>(a);
        case 461:
            return radicalInverseLoop<3271>(a);
        case 462:
            return radicalInverseLoop<3299>(a);
        case 463:
            return radicalInverseLoop<3301>(a);
        case 464:
            return radicalInverseLoop<3307>(a);
        case 465:
            return radicalInverseLoop<3313>(a);
        case 466:
            return radicalInverseLoop<3319>(a);
        case 467:
            return radicalInverseLoop<3323>(a);
        case 468:
            return radicalInverseLoop<3329>(a);
        case 469:
            return radicalInverseLoop<3331>(a);
        case 470:
            return radicalInverseLoop<3343>(a);
        case 471:
            return radicalInverseLoop<3347>(a);
        case 472:
            return radicalInverseLoop<3359>(a);
        case 473:
            return radicalInverseLoop<3361>(a);
        case 474:
            return radicalInverseLoop<3371>(a);
        case 475:
            return radicalInverseLoop<3373>(a);
        case 476:
            return radicalInverseLoop<3389>(a);
        case 477:
            return radicalInverseLoop<3391>(a);
        case 478:
            return radicalInverseLoop<3407>(a);
        case 479:
            return radicalInverseLoop<3413>(a);
        case 480:
            return radicalInverseLoop<3433>(a);
        case 481:
            return radicalInverseLoop<3449>(a);
        case 482:
            return radicalInverseLoop<3457>(a);
        case 483:
            return radicalInverseLoop<3461>(a);
        case 484:
            return radicalInverseLoop<3463>(a);
        case 485:
            return radicalInverseLoop<3467>(a);
        case 486:
            return radicalInverseLoop<3469>(a);
        case 487:
            return radicalInverseLoop<3491>(a);
        case 488:
            return radicalInverseLoop<3499>(a);
        case 489:
            return radicalInverseLoop<3511>(a);
        case 490:
            return radicalInverseLoop<3517>(a);
        case 491:
            return radicalInverseLoop<3527>(a);
        case 492:
            return radicalInverseLoop<3529>(a);
        case 493:
            return radicalInverseLoop<3533>(a);
        case 494:
            return radicalInverseLoop<3539>(a);
        case 495:
            return radicalInverseLoop<3541>(a);
        case 496:
            return radicalInverseLoop<3547>(a);
        case 497:
            return radicalInverseLoop<3557>(a);
        case 498:
            return radicalInverseLoop<3559>(a);
        case 499:
            return radicalInverseLoop<3571>(a);
        case 500:
            return radicalInverseLoop<3581>(a);
        case 501:
            return radicalInverseLoop<3583>(a);
        case 502:
            return radicalInverseLoop<3593>(a);
        case 503:
            return radicalInverseLoop<3607>(a);
        case 504:
            return radicalInverseLoop<3613>(a);
        case 505:
            return radicalInverseLoop<3617>(a);
        case 506:
            return radicalInverseLoop<3623>(a);
        case 507:
            return radicalInverseLoop<3631>(a);
        case 508:
            return radicalInverseLoop<3637>(a);
        case 509:
            return radicalInverseLoop<3643>(a);
        case 510:
            return radicalInverseLoop<3659>(a);
        case 511:
            return radicalInverseLoop<3671>(a);
        case 512:
            return radicalInverseLoop<3673>(a);
        case 513:
            return radicalInverseLoop<3677>(a);
        case 514:
            return radicalInverseLoop<3691>(a);
        case 515:
            return radicalInverseLoop<3697>(a);
        case 516:
            return radicalInverseLoop<3701>(a);
        case 517:
            return radicalInverseLoop<3709>(a);
        case 518:
            return radicalInverseLoop<3719>(a);
        case 519:
            return radicalInverseLoop<3727>(a);
        case 520:
            return radicalInverseLoop<3733>(a);
        case 521:
            return radicalInverseLoop<3739>(a);
        case 522:
            return radicalInverseLoop<3761>(a);
        case 523:
            return radicalInverseLoop<3767>(a);
        case 524:
            return radicalInverseLoop<3769>(a);
        case 525:
            return radicalInverseLoop<3779>(a);
        case 526:
            return radicalInverseLoop<3793>(a);
        case 527:
            return radicalInverseLoop<3797>(a);
        case 528:
            return radicalInverseLoop<3803>(a);
        case 529:
            return radicalInverseLoop<3821>(a);
        case 530:
            return radicalInverseLoop<3823>(a);
        case 531:
            return radicalInverseLoop<3833>(a);
        case 532:
            return radicalInverseLoop<3847>(a);
        case 533:
            return radicalInverseLoop<3851>(a);
        case 534:
            return radicalInverseLoop<3853>(a);
        case 535:
            return radicalInverseLoop<3863>(a);
        case 536:
            return radicalInverseLoop<3877>(a);
        case 537:
            return radicalInverseLoop<3881>(a);
        case 538:
            return radicalInverseLoop<3889>(a);
        case 539:
            return radicalInverseLoop<3907>(a);
        case 540:
            return radicalInverseLoop<3911>(a);
        case 541:
            return radicalInverseLoop<3917>(a);
        case 542:
            return radicalInverseLoop<3919>(a);
        case 543:
            return radicalInverseLoop<3923>(a);
        case 544:
            return radicalInverseLoop<3929>(a);
        case 545:
            return radicalInverseLoop<3931>(a);
        case 546:
            return radicalInverseLoop<3943>(a);
        case 547:
            return radicalInverseLoop<3947>(a);
        case 548:
            return radicalInverseLoop<3967>(a);
        case 549:
            return radicalInverseLoop<3989>(a);
        case 550:
            return radicalInverseLoop<4001>(a);
        case 551:
            return radicalInverseLoop<4003>(a);
        case 552:
            return radicalInverseLoop<4007>(a);
        case 553:
            return radicalInverseLoop<4013>(a);
        case 554:
            return radicalInverseLoop<4019>(a);
        case 555:
            return radicalInverseLoop<4021>(a);
        case 556:
            return radicalInverseLoop<4027>(a);
        case 557:
            return radicalInverseLoop<4049>(a);
        case 558:
            return radicalInverseLoop<4051>(a);
        case 559:
            return radicalInverseLoop<4057>(a);
        case 560:
            return radicalInverseLoop<4073>(a);
        case 561:
            return radicalInverseLoop<4079>(a);
        case 562:
            return radicalInverseLoop<4091>(a);
        case 563:
            return radicalInverseLoop<4093>(a);
        case 564:
            return radicalInverseLoop<4099>(a);
        case 565:
            return radicalInverseLoop<4111>(a);
        case 566:
            return radicalInverseLoop<4127>(a);
        case 567:
            return radicalInverseLoop<4129>(a);
        case 568:
            return radicalInverseLoop<4133>(a);
        case 569:
            return radicalInverseLoop<4139>(a);
        case 570:
            return radicalInverseLoop<4153>(a);
        case 571:
            return radicalInverseLoop<4157>(a);
        case 572:
            return radicalInverseLoop<4159>(a);
        case 573:
            return radicalInverseLoop<4177>(a);
        case 574:
            return radicalInverseLoop<4201>(a);
        case 575:
            return radicalInverseLoop<4211>(a);
        case 576:
            return radicalInverseLoop<4217>(a);
        case 577:
            return radicalInverseLoop<4219>(a);
        case 578:
            return radicalInverseLoop<4229>(a);
        case 579:
            return radicalInverseLoop<4231>(a);
        case 580:
            return radicalInverseLoop<4241>(a);
        case 581:
            return radicalInverseLoop<4243>(a);
        case 582:
            return radicalInverseLoop<4253>(a);
        case 583:
            return radicalInverseLoop<4259>(a);
        case 584:
            return radicalInverseLoop<4261>(a);
        case 585:
            return radicalInverseLoop<4271>(a);
        case 586:
            return radicalInverseLoop<4273>(a);
        case 587:
            return radicalInverseLoop<4283>(a);
        case 588:
            return radicalInverseLoop<4289>(a);
        case 589:
            return radicalInverseLoop<4297>(a);
        case 590:
            return radicalInverseLoop<4327>(a);
        case 591:
            return radicalInverseLoop<4337>(a);
        case 592:
            return radicalInverseLoop<4339>(a);
        case 593:
            return radicalInverseLoop<4349>(a);
        case 594:
            return radicalInverseLoop<4357>(a);
        case 595:
            return radicalInverseLoop<4363>(a);
        case 596:
            return radicalInverseLoop<4373>(a);
        case 597:
            return radicalInverseLoop<4391>(a);
        case 598:
            return radicalInverseLoop<4397>(a);
        case 599:
            return radicalInverseLoop<4409>(a);
        case 600:
            return radicalInverseLoop<4421>(a);
        case 601:
            return radicalInverseLoop<4423>(a);
        case 602:
            return radicalInverseLoop<4441>(a);
        case 603:
            return radicalInverseLoop<4447>(a);
        case 604:
            return radicalInverseLoop<4451>(a);
        case 605:
            return radicalInverseLoop<4457>(a);
        case 606:
            return radicalInverseLoop<4463>(a);
        case 607:
            return radicalInverseLoop<4481>(a);
        case 608:
            return radicalInverseLoop<4483>(a);
        case 609:
            return radicalInverseLoop<4493>(a);
        case 610:
            return radicalInverseLoop<4507>(a);
        case 611:
            return radicalInverseLoop<4513>(a);
        case 612:
            return radicalInverseLoop<4517>(a);
        case 613:
            return radicalInverseLoop<4519>(a);
        case 614:
            return radicalInverseLoop<4523>(a);
        case 615:
            return radicalInverseLoop<4547>(a);
        case 616:
            return radicalInverseLoop<4549>(a);
        case 617:
            return radicalInverseLoop<4561>(a);
        case 618:
            return radicalInverseLoop<4567>(a);
        case 619:
            return radicalInverseLoop<4583>(a);
        case 620:
            return radicalInverseLoop<4591>(a);
        case 621:
            return radicalInverseLoop<4597>(a);
        case 622:
            return radicalInverseLoop<4603>(a);
        case 623:
            return radicalInverseLoop<4621>(a);
        case 624:
            return radicalInverseLoop<4637>(a);
        case 625:
            return radicalInverseLoop<4639>(a);
        case 626:
            return radicalInverseLoop<4643>(a);
        case 627:
            return radicalInverseLoop<4649>(a);
        case 628:
            return radicalInverseLoop<4651>(a);
        case 629:
            return radicalInverseLoop<4657>(a);
        case 630:
            return radicalInverseLoop<4663>(a);
        case 631:
            return radicalInverseLoop<4673>(a);
        case 632:
            return radicalInverseLoop<4679>(a);
        case 633:
            return radicalInverseLoop<4691>(a);
        case 634:
            return radicalInverseLoop<4703>(a);
        case 635:
            return radicalInverseLoop<4721>(a);
        case 636:
            return radicalInverseLoop<4723>(a);
        case 637:
            return radicalInverseLoop<4729>(a);
        case 638:
            return radicalInverseLoop<4733>(a);
        case 639:
            return radicalInverseLoop<4751>(a);
        case 640:
            return radicalInverseLoop<4759>(a);
        case 641:
            return radicalInverseLoop<4783>(a);
        case 642:
            return radicalInverseLoop<4787>(a);
        case 643:
            return radicalInverseLoop<4789>(a);
        case 644:
            return radicalInverseLoop<4793>(a);
        case 645:
            return radicalInverseLoop<4799>(a);
        case 646:
            return radicalInverseLoop<4801>(a);
        case 647:
            return radicalInverseLoop<4813>(a);
        case 648:
            return radicalInverseLoop<4817>(a);
        case 649:
            return radicalInverseLoop<4831>(a);
        case 650:
            return radicalInverseLoop<4861>(a);
        case 651:
            return radicalInverseLoop<4871>(a);
        case 652:
            return radicalInverseLoop<4877>(a);
        case 653:
            return radicalInverseLoop<4889>(a);
        case 654:
            return radicalInverseLoop<4903>(a);
        case 655:
            return radicalInverseLoop<4909>(a);
        case 656:
            return radicalInverseLoop<4919>(a);
        case 657:
            return radicalInverseLoop<4931>(a);
        case 658:
            return radicalInverseLoop<4933>(a);
        case 659:
            return radicalInverseLoop<4937>(a);
        case 660:
            return radicalInverseLoop<4943>(a);
        case 661:
            return radicalInverseLoop<4951>(a);
        case 662:
            return radicalInverseLoop<4957>(a);
        case 663:
            return radicalInverseLoop<4967>(a);
        case 664:
            return radicalInverseLoop<4969>(a);
        case 665:
            return radicalInverseLoop<4973>(a);
        case 666:
            return radicalInverseLoop<4987>(a);
        case 667:
            return radicalInverseLoop<4993>(a);
        case 668:
            return radicalInverseLoop<4999>(a);
        case 669:
            return radicalInverseLoop<5003>(a);
        case 670:
            return radicalInverseLoop<5009>(a);
        case 671:
            return radicalInverseLoop<5011>(a);
        case 672:
            return radicalInverseLoop<5021>(a);
        case 673:
            return radicalInverseLoop<5023>(a);
        case 674:
            return radicalInverseLoop<5039>(a);
        case 675:
            return radicalInverseLoop<5051>(a);
        case 676:
            return radicalInverseLoop<5059>(a);
        case 677:
            return radicalInverseLoop<5077>(a);
        case 678:
            return radicalInverseLoop<5081>(a);
        case 679:
            return radicalInverseLoop<5087>(a);
        case 680:
            return radicalInverseLoop<5099>(a);
        case 681:
            return radicalInverseLoop<5101>(a);
        case 682:
            return radicalInverseLoop<5107>(a);
        case 683:
            return radicalInverseLoop<5113>(a);
        case 684:
            return radicalInverseLoop<5119>(a);
        case 685:
            return radicalInverseLoop<5147>(a);
        case 686:
            return radicalInverseLoop<5153>(a);
        case 687:
            return radicalInverseLoop<5167>(a);
        case 688:
            return radicalInverseLoop<5171>(a);
        case 689:
            return radicalInverseLoop<5179>(a);
        case 690:
            return radicalInverseLoop<5189>(a);
        case 691:
            return radicalInverseLoop<5197>(a);
        case 692:
            return radicalInverseLoop<5209>(a);
        case 693:
            return radicalInverseLoop<5227>(a);
        case 694:
            return radicalInverseLoop<5231>(a);
        case 695:
            return radicalInverseLoop<5233>(a);
        case 696:
            return radicalInverseLoop<5237>(a);
        case 697:
            return radicalInverseLoop<5261>(a);
        case 698:
            return radicalInverseLoop<5273>(a);
        case 699:
            return radicalInverseLoop<5279>(a);
        case 700:
            return radicalInverseLoop<5281>(a);
        case 701:
            return radicalInverseLoop<5297>(a);
        case 702:
            return radicalInverseLoop<5303>(a);
        case 703:
            return radicalInverseLoop<5309>(a);
        case 704:
            return radicalInverseLoop<5323>(a);
        case 705:
            return radicalInverseLoop<5333>(a);
        case 706:
            return radicalInverseLoop<5347>(a);
        case 707:
            return radicalInverseLoop<5351>(a);
        case 708:
            return radicalInverseLoop<5381>(a);
        case 709:
            return radicalInverseLoop<5387>(a);
        case 710:
            return radicalInverseLoop<5393>(a);
        case 711:
            return radicalInverseLoop<5399>(a);
        case 712:
            return radicalInverseLoop<5407>(a);
        case 713:
            return radicalInverseLoop<5413>(a);
        case 714:
            return radicalInverseLoop<5417>(a);
        case 715:
            return radicalInverseLoop<5419>(a);
        case 716:
            return radicalInverseLoop<5431>(a);
        case 717:
            return radicalInverseLoop<5437>(a);
        case 718:
            return radicalInverseLoop<5441>(a);
        case 719:
            return radicalInverseLoop<5443>(a);
        case 720:
            return radicalInverseLoop<5449>(a);
        case 721:
            return radicalInverseLoop<5471>(a);
        case 722:
            return radicalInverseLoop<5477>(a);
        case 723:
            return radicalInverseLoop<5479>(a);
        case 724:
            return radicalInverseLoop<5483>(a);
        case 725:
            return radicalInverseLoop<5501>(a);
        case 726:
            return radicalInverseLoop<5503>(a);
        case 727:
            return radicalInverseLoop<5507>(a);
        case 728:
            return radicalInverseLoop<5519>(a);
        case 729:
            return radicalInverseLoop<5521>(a);
        case 730:
            return radicalInverseLoop<5527>(a);
        case 731:
            return radicalInverseLoop<5531>(a);
        case 732:
            return radicalInverseLoop<5557>(a);
        case 733:
            return radicalInverseLoop<5563>(a);
        case 734:
            return radicalInverseLoop<5569>(a);
        case 735:
            return radicalInverseLoop<5573>(a);
        case 736:
            return radicalInverseLoop<5581>(a);
        case 737:
            return radicalInverseLoop<5591>(a);
        case 738:
            return radicalInverseLoop<5623>(a);
        case 739:
            return radicalInverseLoop<5639>(a);
        case 740:
            return radicalInverseLoop<5641>(a);
        case 741:
            return radicalInverseLoop<5647>(a);
        case 742:
            return radicalInverseLoop<5651>(a);
        case 743:
            return radicalInverseLoop<5653>(a);
        case 744:
            return radicalInverseLoop<5657>(a);
        case 745:
            return radicalInverseLoop<5659>(a);
        case 746:
            return radicalInverseLoop<5669>(a);
        case 747:
            return radicalInverseLoop<5683>(a);
        case 748:
            return radicalInverseLoop<5689>(a);
        case 749:
            return radicalInverseLoop<5693>(a);
        case 750:
            return radicalInverseLoop<5701>(a);
        case 751:
            return radicalInverseLoop<5711>(a);
        case 752:
            return radicalInverseLoop<5717>(a);
        case 753:
            return radicalInverseLoop<5737>(a);
        case 754:
            return radicalInverseLoop<5741>(a);
        case 755:
            return radicalInverseLoop<5743>(a);
        case 756:
            return radicalInverseLoop<5749>(a);
        case 757:
            return radicalInverseLoop<5779>(a);
        case 758:
            return radicalInverseLoop<5783>(a);
        case 759:
            return radicalInverseLoop<5791>(a);
        case 760:
            return radicalInverseLoop<5801>(a);
        case 761:
            return radicalInverseLoop<5807>(a);
        case 762:
            return radicalInverseLoop<5813>(a);
        case 763:
            return radicalInverseLoop<5821>(a);
        case 764:
            return radicalInverseLoop<5827>(a);
        case 765:
            return radicalInverseLoop<5839>(a);
        case 766:
            return radicalInverseLoop<5843>(a);
        case 767:
            return radicalInverseLoop<5849>(a);
        case 768:
            return radicalInverseLoop<5851>(a);
        case 769:
            return radicalInverseLoop<5857>(a);
        case 770:
            return radicalInverseLoop<5861>(a);
        case 771:
            return radicalInverseLoop<5867>(a);
        case 772:
            return radicalInverseLoop<5869>(a);
        case 773:
            return radicalInverseLoop<5879>(a);
        case 774:
            return radicalInverseLoop<5881>(a);
        case 775:
            return radicalInverseLoop<5897>(a);
        case 776:
            return radicalInverseLoop<5903>(a);
        case 777:
            return radicalInverseLoop<5923>(a);
        case 778:
            return radicalInverseLoop<5927>(a);
        case 779:
            return radicalInverseLoop<5939>(a);
        case 780:
            return radicalInverseLoop<5953>(a);
        case 781:
            return radicalInverseLoop<5981>(a);
        case 782:
            return radicalInverseLoop<5987>(a);
        case 783:
            return radicalInverseLoop<6007>(a);
        case 784:
            return radicalInverseLoop<6011>(a);
        case 785:
            return radicalInverseLoop<6029>(a);
        case 786:
            return radicalInverseLoop<6037>(a);
        case 787:
            return radicalInverseLoop<6043>(a);
        case 788:
            return radicalInverseLoop<6047>(a);
        case 789:
            return radicalInverseLoop<6053>(a);
        case 790:
            return radicalInverseLoop<6067>(a);
        case 791:
            return radicalInverseLoop<6073>(a);
        case 792:
            return radicalInverseLoop<6079>(a);
        case 793:
            return radicalInverseLoop<6089>(a);
        case 794:
            return radicalInverseLoop<6091>(a);
        case 795:
            return radicalInverseLoop<6101>(a);
        case 796:
            return radicalInverseLoop<6113>(a);
        case 797:
            return radicalInverseLoop<6121>(a);
        case 798:
            return radicalInverseLoop<6131>(a);
        case 799:
            return radicalInverseLoop<6133>(a);
        case 800:
            return radicalInverseLoop<6143>(a);
        case 801:
            return radicalInverseLoop<6151>(a);
        case 802:
            return radicalInverseLoop<6163>(a);
        case 803:
            return radicalInverseLoop<6173>(a);
        case 804:
            return radicalInverseLoop<6197>(a);
        case 805:
            return radicalInverseLoop<6199>(a);
        case 806:
            return radicalInverseLoop<6203>(a);
        case 807:
            return radicalInverseLoop<6211>(a);
        case 808:
            return radicalInverseLoop<6217>(a);
        case 809:
            return radicalInverseLoop<6221>(a);
        case 810:
            return radicalInverseLoop<6229>(a);
        case 811:
            return radicalInverseLoop<6247>(a);
        case 812:
            return radicalInverseLoop<6257>(a);
        case 813:
            return radicalInverseLoop<6263>(a);
        case 814:
            return radicalInverseLoop<6269>(a);
        case 815:
            return radicalInverseLoop<6271>(a);
        case 816:
            return radicalInverseLoop<6277>(a);
        case 817:
            return radicalInverseLoop<6287>(a);
        case 818:
            return radicalInverseLoop<6299>(a);
        case 819:
            return radicalInverseLoop<6301>(a);
        case 820:
            return radicalInverseLoop<6311>(a);
        case 821:
            return radicalInverseLoop<6317>(a);
        case 822:
            return radicalInverseLoop<6323>(a);
        case 823:
            return radicalInverseLoop<6329>(a);
        case 824:
            return radicalInverseLoop<6337>(a);
        case 825:
            return radicalInverseLoop<6343>(a);
        case 826:
            return radicalInverseLoop<6353>(a);
        case 827:
            return radicalInverseLoop<6359>(a);
        case 828:
            return radicalInverseLoop<6361>(a);
        case 829:
            return radicalInverseLoop<6367>(a);
        case 830:
            return radicalInverseLoop<6373>(a);
        case 831:
            return radicalInverseLoop<6379>(a);
        case 832:
            return radicalInverseLoop<6389>(a);
        case 833:
            return radicalInverseLoop<6397>(a);
        case 834:
            return radicalInverseLoop<6421>(a);
        case 835:
            return radicalInverseLoop<6427>(a);
        case 836:
            return radicalInverseLoop<6449>(a);
        case 837:
            return radicalInverseLoop<6451>(a);
        case 838:
            return radicalInverseLoop<6469>(a);
        case 839:
            return radicalInverseLoop<6473>(a);
        case 840:
            return radicalInverseLoop<6481>(a);
        case 841:
            return radicalInverseLoop<6491>(a);
        case 842:
            return radicalInverseLoop<6521>(a);
        case 843:
            return radicalInverseLoop<6529>(a);
        case 844:
            return radicalInverseLoop<6547>(a);
        case 845:
            return radicalInverseLoop<6551>(a);
        case 846:
            return radicalInverseLoop<6553>(a);
        case 847:
            return radicalInverseLoop<6563>(a);
        case 848:
            return radicalInverseLoop<6569>(a);
        case 849:
            return radicalInverseLoop<6571>(a);
        case 850:
            return radicalInverseLoop<6577>(a);
        case 851:
            return radicalInverseLoop<6581>(a);
        case 852:
            return radicalInverseLoop<6599>(a);
        case 853:
            return radicalInverseLoop<6607>(a);
        case 854:
            return radicalInverseLoop<6619>(a);
        case 855:
            return radicalInverseLoop<6637>(a);
        case 856:
            return radicalInverseLoop<6653>(a);
        case 857:
            return radicalInverseLoop<6659>(a);
        case 858:
            return radicalInverseLoop<6661>(a);
        case 859:
            return radicalInverseLoop<6673>(a);
        case 860:
            return radicalInverseLoop<6679>(a);
        case 861:
            return radicalInverseLoop<6689>(a);
        case 862:
            return radicalInverseLoop<6691>(a);
        case 863:
            return radicalInverseLoop<6701>(a);
        case 864:
            return radicalInverseLoop<6703>(a);
        case 865:
            return radicalInverseLoop<6709>(a);
        case 866:
            return radicalInverseLoop<6719>(a);
        case 867:
            return radicalInverseLoop<6733>(a);
        case 868:
            return radicalInverseLoop<6737>(a);
        case 869:
            return radicalInverseLoop<6761>(a);
        case 870:
            return radicalInverseLoop<6763>(a);
        case 871:
            return radicalInverseLoop<6779>(a);
        case 872:
            return radicalInverseLoop<6781>(a);
        case 873:
            return radicalInverseLoop<6791>(a);
        case 874:
            return radicalInverseLoop<6793>(a);
        case 875:
            return radicalInverseLoop<6803>(a);
        case 876:
            return radicalInverseLoop<6823>(a);
        case 877:
            return radicalInverseLoop<6827>(a);
        case 878:
            return radicalInverseLoop<6829>(a);
        case 879:
            return radicalInverseLoop<6833>(a);
        case 880:
            return radicalInverseLoop<6841>(a);
        case 881:
            return radicalInverseLoop<6857>(a);
        case 882:
            return radicalInverseLoop<6863>(a);
        case 883:
            return radicalInverseLoop<6869>(a);
        case 884:
            return radicalInverseLoop<6871>(a);
        case 885:
            return radicalInverseLoop<6883>(a);
        case 886:
            return radicalInverseLoop<6899>(a);
        case 887:
            return radicalInverseLoop<6907>(a);
        case 888:
            return radicalInverseLoop<6911>(a);
        case 889:
            return radicalInverseLoop<6917>(a);
        case 890:
            return radicalInverseLoop<6947>(a);
        case 891:
            return radicalInverseLoop<6949>(a);
        case 892:
            return radicalInverseLoop<6959>(a);
        case 893:
            return radicalInverseLoop<6961>(a);
        case 894:
            return radicalInverseLoop<6967>(a);
        case 895:
            return radicalInverseLoop<6971>(a);
        case 896:
            return radicalInverseLoop<6977>(a);
        case 897:
            return radicalInverseLoop<6983>(a);
        case 898:
            return radicalInverseLoop<6991>(a);
        case 899:
            return radicalInverseLoop<6997>(a);
        case 900:
            return radicalInverseLoop<7001>(a);
        case 901:
            return radicalInverseLoop<7013>(a);
        case 902:
            return radicalInverseLoop<7019>(a);
        case 903:
            return radicalInverseLoop<7027>(a);
        case 904:
            return radicalInverseLoop<7039>(a);
        case 905:
            return radicalInverseLoop<7043>(a);
        case 906:
            return radicalInverseLoop<7057>(a);
        case 907:
            return radicalInverseLoop<7069>(a);
        case 908:
            return radicalInverseLoop<7079>(a);
        case 909:
            return radicalInverseLoop<7103>(a);
        case 910:
            return radicalInverseLoop<7109>(a);
        case 911:
            return radicalInverseLoop<7121>(a);
        case 912:
            return radicalInverseLoop<7127>(a);
        case 913:
            return radicalInverseLoop<7129>(a);
        case 914:
            return radicalInverseLoop<7151>(a);
        case 915:
            return radicalInverseLoop<7159>(a);
        case 916:
            return radicalInverseLoop<7177>(a);
        case 917:
            return radicalInverseLoop<7187>(a);
        case 918:
            return radicalInverseLoop<7193>(a);
        case 919:
            return radicalInverseLoop<7207>(a);
        case 920:
            return radicalInverseLoop<7211>(a);
        case 921:
            return radicalInverseLoop<7213>(a);
        case 922:
            return radicalInverseLoop<7219>(a);
        case 923:
            return radicalInverseLoop<7229>(a);
        case 924:
            return radicalInverseLoop<7237>(a);
        case 925:
            return radicalInverseLoop<7243>(a);
        case 926:
            return radicalInverseLoop<7247>(a);
        case 927:
            return radicalInverseLoop<7253>(a);
        case 928:
            return radicalInverseLoop<7283>(a);
        case 929:
            return radicalInverseLoop<7297>(a);
        case 930:
            return radicalInverseLoop<7307>(a);
        case 931:
            return radicalInverseLoop<7309>(a);
        case 932:
            return radicalInverseLoop<7321>(a);
        case 933:
            return radicalInverseLoop<7331>(a);
        case 934:
            return radicalInverseLoop<7333>(a);
        case 935:
            return radicalInverseLoop<7349>(a);
        case 936:
            return radicalInverseLoop<7351>(a);
        case 937:
            return radicalInverseLoop<7369>(a);
        case 938:
            return radicalInverseLoop<7393>(a);
        case 939:
            return radicalInverseLoop<7411>(a);
        case 940:
            return radicalInverseLoop<7417>(a);
        case 941:
            return radicalInverseLoop<7433>(a);
        case 942:
            return radicalInverseLoop<7451>(a);
        case 943:
            return radicalInverseLoop<7457>(a);
        case 944:
            return radicalInverseLoop<7459>(a);
        case 945:
            return radicalInverseLoop<7477>(a);
        case 946:
            return radicalInverseLoop<7481>(a);
        case 947:
            return radicalInverseLoop<7487>(a);
        case 948:
            return radicalInverseLoop<7489>(a);
        case 949:
            return radicalInverseLoop<7499>(a);
        case 950:
            return radicalInverseLoop<7507>(a);
        case 951:
            return radicalInverseLoop<7517>(a);
        case 952:
            return radicalInverseLoop<7523>(a);
        case 953:
            return radicalInverseLoop<7529>(a);
        case 954:
            return radicalInverseLoop<7537>(a);
        case 955:
            return radicalInverseLoop<7541>(a);
        case 956:
            return radicalInverseLoop<7547>(a);
        case 957:
            return radicalInverseLoop<7549>(a);
        case 958:
            return radicalInverseLoop<7559>(a);
        case 959:
            return radicalInverseLoop<7561>(a);
        case 960:
            return radicalInverseLoop<7573>(a);
        case 961:
            return radicalInverseLoop<7577>(a);
        case 962:
            return radicalInverseLoop<7583>(a);
        case 963:
            return radicalInverseLoop<7589>(a);
        case 964:
            return radicalInverseLoop<7591>(a);
        case 965:
            return radicalInverseLoop<7603>(a);
        case 966:
            return radicalInverseLoop<7607>(a);
        case 967:
            return radicalInverseLoop<7621>(a);
        case 968:
            return radicalInverseLoop<7639>(a);
        case 969:
            return radicalInverseLoop<7643>(a);
        case 970:
            return radicalInverseLoop<7649>(a);
        case 971:
            return radicalInverseLoop<7669>(a);
        case 972:
            return radicalInverseLoop<7673>(a);
        case 973:
            return radicalInverseLoop<7681>(a);
        case 974:
            return radicalInverseLoop<7687>(a);
        case 975:
            return radicalInverseLoop<7691>(a);
        case 976:
            return radicalInverseLoop<7699>(a);
        case 977:
            return radicalInverseLoop<7703>(a);
        case 978:
            return radicalInverseLoop<7717>(a);
        case 979:
            return radicalInverseLoop<7723>(a);
        case 980:
            return radicalInverseLoop<7727>(a);
        case 981:
            return radicalInverseLoop<7741>(a);
        case 982:
            return radicalInverseLoop<7753>(a);
        case 983:
            return radicalInverseLoop<7757>(a);
        case 984:
            return radicalInverseLoop<7759>(a);
        case 985:
            return radicalInverseLoop<7789>(a);
        case 986:
            return radicalInverseLoop<7793>(a);
        case 987:
            return radicalInverseLoop<7817>(a);
        case 988:
            return radicalInverseLoop<7823>(a);
        case 989:
            return radicalInverseLoop<7829>(a);
        case 990:
            return radicalInverseLoop<7841>(a);
        case 991:
            return radicalInverseLoop<7853>(a);
        case 992:
            return radicalInverseLoop<7867>(a);
        case 993:
            return radicalInverseLoop<7873>(a);
        case 994:
            return radicalInverseLoop<7877>(a);
        case 995:
            return radicalInverseLoop<7879>(a);
        case 996:
            return radicalInverseLoop<7883>(a);
        case 997:
            return radicalInverseLoop<7901>(a);
        case 998:
            return radicalInverseLoop<7907>(a);
        case 999:
            return radicalInverseLoop<7919>(a);
        case 1000:
            return radicalInverseLoop<7927>(a);
        case 1001:
            return radicalInverseLoop<7933>(a);
        case 1002:
            return radicalInverseLoop<7937>(a);
        case 1003:
            return radicalInverseLoop<7949>(a);
        case 1004:
            return radicalInverseLoop<7951>(a);
        case 1005:
            return radicalInverseLoop<7963>(a);
        case 1006:
            return radicalInverseLoop<7993>(a);
        case 1007:
            return radicalInverseLoop<8009>(a);
        case 1008:
            return radicalInverseLoop<8011>(a);
        case 1009:
            return radicalInverseLoop<8017>(a);
        case 1010:
            return radicalInverseLoop<8039>(a);
        case 1011:
            return radicalInverseLoop<8053>(a);
        case 1012:
            return radicalInverseLoop<8059>(a);
        case 1013:
            return radicalInverseLoop<8069>(a);
        case 1014:
            return radicalInverseLoop<8081>(a);
        case 1015:
            return radicalInverseLoop<8087>(a);
        case 1016:
            return radicalInverseLoop<8089>(a);
        case 1017:
            return radicalInverseLoop<8093>(a);
        case 1018:
            return radicalInverseLoop<8101>(a);
        case 1019:
            return radicalInverseLoop<8111>(a);
        case 1020:
            return radicalInverseLoop<8117>(a);
        case 1021:
            return radicalInverseLoop<8123>(a);
        case 1022:
            return radicalInverseLoop<8147>(a);
        case 1023:
            return radicalInverseLoop<8161>(a);
    }
    return 0;
}
double radicalInverseScramble(int baseIndex, uint64_t a, const uint16_t *perm) {
    assert(0 <= baseIndex && baseIndex < 1024);
    switch (baseIndex) {
        case 0:
            return radicalInverseScrambleLoop<2>(perm, a);
        case 1:
            return radicalInverseScrambleLoop<3>(perm, a);
        case 2:
            return radicalInverseScrambleLoop<5>(perm, a);
        case 3:
            return radicalInverseScrambleLoop<7>(perm, a);
        case 4:
            return radicalInverseScrambleLoop<11>(perm, a);
        case 5:
            return radicalInverseScrambleLoop<13>(perm, a);
        case 6:
            return radicalInverseScrambleLoop<17>(perm, a);
        case 7:
            return radicalInverseScrambleLoop<19>(perm, a);
        case 8:
            return radicalInverseScrambleLoop<23>(perm, a);
        case 9:
            return radicalInverseScrambleLoop<29>(perm, a);
        case 10:
            return radicalInverseScrambleLoop<31>(perm, a);
        case 11:
            return radicalInverseScrambleLoop<37>(perm, a);
        case 12:
            return radicalInverseScrambleLoop<41>(perm, a);
        case 13:
            return radicalInverseScrambleLoop<43>(perm, a);
        case 14:
            return radicalInverseScrambleLoop<47>(perm, a);
        case 15:
            return radicalInverseScrambleLoop<53>(perm, a);
        case 16:
            return radicalInverseScrambleLoop<59>(perm, a);
        case 17:
            return radicalInverseScrambleLoop<61>(perm, a);
        case 18:
            return radicalInverseScrambleLoop<67>(perm, a);
        case 19:
            return radicalInverseScrambleLoop<71>(perm, a);
        case 20:
            return radicalInverseScrambleLoop<73>(perm, a);
        case 21:
            return radicalInverseScrambleLoop<79>(perm, a);
        case 22:
            return radicalInverseScrambleLoop<83>(perm, a);
        case 23:
            return radicalInverseScrambleLoop<89>(perm, a);
        case 24:
            return radicalInverseScrambleLoop<97>(perm, a);
        case 25:
            return radicalInverseScrambleLoop<101>(perm, a);
        case 26:
            return radicalInverseScrambleLoop<103>(perm, a);
        case 27:
            return radicalInverseScrambleLoop<107>(perm, a);
        case 28:
            return radicalInverseScrambleLoop<109>(perm, a);
        case 29:
            return radicalInverseScrambleLoop<113>(perm, a);
        case 30:
            return radicalInverseScrambleLoop<127>(perm, a);
        case 31:
            return radicalInverseScrambleLoop<131>(perm, a);
        case 32:
            return radicalInverseScrambleLoop<137>(perm, a);
        case 33:
            return radicalInverseScrambleLoop<139>(perm, a);
        case 34:
            return radicalInverseScrambleLoop<149>(perm, a);
        case 35:
            return radicalInverseScrambleLoop<151>(perm, a);
        case 36:
            return radicalInverseScrambleLoop<157>(perm, a);
        case 37:
            return radicalInverseScrambleLoop<163>(perm, a);
        case 38:
            return radicalInverseScrambleLoop<167>(perm, a);
        case 39:
            return radicalInverseScrambleLoop<173>(perm, a);
        case 40:
            return radicalInverseScrambleLoop<179>(perm, a);
        case 41:
            return radicalInverseScrambleLoop<181>(perm, a);
        case 42:
            return radicalInverseScrambleLoop<191>(perm, a);
        case 43:
            return radicalInverseScrambleLoop<193>(perm, a);
        case 44:
            return radicalInverseScrambleLoop<197>(perm, a);
        case 45:
            return radicalInverseScrambleLoop<199>(perm, a);
        case 46:
            return radicalInverseScrambleLoop<211>(perm, a);
        case 47:
            return radicalInverseScrambleLoop<223>(perm, a);
        case 48:
            return radicalInverseScrambleLoop<227>(perm, a);
        case 49:
            return radicalInverseScrambleLoop<229>(perm, a);
        case 50:
            return radicalInverseScrambleLoop<233>(perm, a);
        case 51:
            return radicalInverseScrambleLoop<239>(perm, a);
        case 52:
            return radicalInverseScrambleLoop<241>(perm, a);
        case 53:
            return radicalInverseScrambleLoop<251>(perm, a);
        case 54:
            return radicalInverseScrambleLoop<257>(perm, a);
        case 55:
            return radicalInverseScrambleLoop<263>(perm, a);
        case 56:
            return radicalInverseScrambleLoop<269>(perm, a);
        case 57:
            return radicalInverseScrambleLoop<271>(perm, a);
        case 58:
            return radicalInverseScrambleLoop<277>(perm, a);
        case 59:
            return radicalInverseScrambleLoop<281>(perm, a);
        case 60:
            return radicalInverseScrambleLoop<283>(perm, a);
        case 61:
            return radicalInverseScrambleLoop<293>(perm, a);
        case 62:
            return radicalInverseScrambleLoop<307>(perm, a);
        case 63:
            return radicalInverseScrambleLoop<311>(perm, a);
        case 64:
            return radicalInverseScrambleLoop<313>(perm, a);
        case 65:
            return radicalInverseScrambleLoop<317>(perm, a);
        case 66:
            return radicalInverseScrambleLoop<331>(perm, a);
        case 67:
            return radicalInverseScrambleLoop<337>(perm, a);
        case 68:
            return radicalInverseScrambleLoop<347>(perm, a);
        case 69:
            return radicalInverseScrambleLoop<349>(perm, a);
        case 70:
            return radicalInverseScrambleLoop<353>(perm, a);
        case 71:
            return radicalInverseScrambleLoop<359>(perm, a);
        case 72:
            return radicalInverseScrambleLoop<367>(perm, a);
        case 73:
            return radicalInverseScrambleLoop<373>(perm, a);
        case 74:
            return radicalInverseScrambleLoop<379>(perm, a);
        case 75:
            return radicalInverseScrambleLoop<383>(perm, a);
        case 76:
            return radicalInverseScrambleLoop<389>(perm, a);
        case 77:
            return radicalInverseScrambleLoop<397>(perm, a);
        case 78:
            return radicalInverseScrambleLoop<401>(perm, a);
        case 79:
            return radicalInverseScrambleLoop<409>(perm, a);
        case 80:
            return radicalInverseScrambleLoop<419>(perm, a);
        case 81:
            return radicalInverseScrambleLoop<421>(perm, a);
        case 82:
            return radicalInverseScrambleLoop<431>(perm, a);
        case 83:
            return radicalInverseScrambleLoop<433>(perm, a);
        case 84:
            return radicalInverseScrambleLoop<439>(perm, a);
        case 85:
            return radicalInverseScrambleLoop<443>(perm, a);
        case 86:
            return radicalInverseScrambleLoop<449>(perm, a);
        case 87:
            return radicalInverseScrambleLoop<457>(perm, a);
        case 88:
            return radicalInverseScrambleLoop<461>(perm, a);
        case 89:
            return radicalInverseScrambleLoop<463>(perm, a);
        case 90:
            return radicalInverseScrambleLoop<467>(perm, a);
        case 91:
            return radicalInverseScrambleLoop<479>(perm, a);
        case 92:
            return radicalInverseScrambleLoop<487>(perm, a);
        case 93:
            return radicalInverseScrambleLoop<491>(perm, a);
        case 94:
            return radicalInverseScrambleLoop<499>(perm, a);
        case 95:
            return radicalInverseScrambleLoop<503>(perm, a);
        case 96:
            return radicalInverseScrambleLoop<509>(perm, a);
        case 97:
            return radicalInverseScrambleLoop<521>(perm, a);
        case 98:
            return radicalInverseScrambleLoop<523>(perm, a);
        case 99:
            return radicalInverseScrambleLoop<541>(perm, a);
        case 100:
            return radicalInverseScrambleLoop<547>(perm, a);
        case 101:
            return radicalInverseScrambleLoop<557>(perm, a);
        case 102:
            return radicalInverseScrambleLoop<563>(perm, a);
        case 103:
            return radicalInverseScrambleLoop<569>(perm, a);
        case 104:
            return radicalInverseScrambleLoop<571>(perm, a);
        case 105:
            return radicalInverseScrambleLoop<577>(perm, a);
        case 106:
            return radicalInverseScrambleLoop<587>(perm, a);
        case 107:
            return radicalInverseScrambleLoop<593>(perm, a);
        case 108:
            return radicalInverseScrambleLoop<599>(perm, a);
        case 109:
            return radicalInverseScrambleLoop<601>(perm, a);
        case 110:
            return radicalInverseScrambleLoop<607>(perm, a);
        case 111:
            return radicalInverseScrambleLoop<613>(perm, a);
        case 112:
            return radicalInverseScrambleLoop<617>(perm, a);
        case 113:
            return radicalInverseScrambleLoop<619>(perm, a);
        case 114:
            return radicalInverseScrambleLoop<631>(perm, a);
        case 115:
            return radicalInverseScrambleLoop<641>(perm, a);
        case 116:
            return radicalInverseScrambleLoop<643>(perm, a);
        case 117:
            return radicalInverseScrambleLoop<647>(perm, a);
        case 118:
            return radicalInverseScrambleLoop<653>(perm, a);
        case 119:
            return radicalInverseScrambleLoop<659>(perm, a);
        case 120:
            return radicalInverseScrambleLoop<661>(perm, a);
        case 121:
            return radicalInverseScrambleLoop<673>(perm, a);
        case 122:
            return radicalInverseScrambleLoop<677>(perm, a);
        case 123:
            return radicalInverseScrambleLoop<683>(perm, a);
        case 124:
            return radicalInverseScrambleLoop<691>(perm, a);
        case 125:
            return radicalInverseScrambleLoop<701>(perm, a);
        case 126:
            return radicalInverseScrambleLoop<709>(perm, a);
        case 127:
            return radicalInverseScrambleLoop<719>(perm, a);
        case 128:
            return radicalInverseScrambleLoop<727>(perm, a);
        case 129:
            return radicalInverseScrambleLoop<733>(perm, a);
        case 130:
            return radicalInverseScrambleLoop<739>(perm, a);
        case 131:
            return radicalInverseScrambleLoop<743>(perm, a);
        case 132:
            return radicalInverseScrambleLoop<751>(perm, a);
        case 133:
            return radicalInverseScrambleLoop<757>(perm, a);
        case 134:
            return radicalInverseScrambleLoop<761>(perm, a);
        case 135:
            return radicalInverseScrambleLoop<769>(perm, a);
        case 136:
            return radicalInverseScrambleLoop<773>(perm, a);
        case 137:
            return radicalInverseScrambleLoop<787>(perm, a);
        case 138:
            return radicalInverseScrambleLoop<797>(perm, a);
        case 139:
            return radicalInverseScrambleLoop<809>(perm, a);
        case 140:
            return radicalInverseScrambleLoop<811>(perm, a);
        case 141:
            return radicalInverseScrambleLoop<821>(perm, a);
        case 142:
            return radicalInverseScrambleLoop<823>(perm, a);
        case 143:
            return radicalInverseScrambleLoop<827>(perm, a);
        case 144:
            return radicalInverseScrambleLoop<829>(perm, a);
        case 145:
            return radicalInverseScrambleLoop<839>(perm, a);
        case 146:
            return radicalInverseScrambleLoop<853>(perm, a);
        case 147:
            return radicalInverseScrambleLoop<857>(perm, a);
        case 148:
            return radicalInverseScrambleLoop<859>(perm, a);
        case 149:
            return radicalInverseScrambleLoop<863>(perm, a);
        case 150:
            return radicalInverseScrambleLoop<877>(perm, a);
        case 151:
            return radicalInverseScrambleLoop<881>(perm, a);
        case 152:
            return radicalInverseScrambleLoop<883>(perm, a);
        case 153:
            return radicalInverseScrambleLoop<887>(perm, a);
        case 154:
            return radicalInverseScrambleLoop<907>(perm, a);
        case 155:
            return radicalInverseScrambleLoop<911>(perm, a);
        case 156:
            return radicalInverseScrambleLoop<919>(perm, a);
        case 157:
            return radicalInverseScrambleLoop<929>(perm, a);
        case 158:
            return radicalInverseScrambleLoop<937>(perm, a);
        case 159:
            return radicalInverseScrambleLoop<941>(perm, a);
        case 160:
            return radicalInverseScrambleLoop<947>(perm, a);
        case 161:
            return radicalInverseScrambleLoop<953>(perm, a);
        case 162:
            return radicalInverseScrambleLoop<967>(perm, a);
        case 163:
            return radicalInverseScrambleLoop<971>(perm, a);
        case 164:
            return radicalInverseScrambleLoop<977>(perm, a);
        case 165:
            return radicalInverseScrambleLoop<983>(perm, a);
        case 166:
            return radicalInverseScrambleLoop<991>(perm, a);
        case 167:
            return radicalInverseScrambleLoop<997>(perm, a);
        case 168:
            return radicalInverseScrambleLoop<1009>(perm, a);
        case 169:
            return radicalInverseScrambleLoop<1013>(perm, a);
        case 170:
            return radicalInverseScrambleLoop<1019>(perm, a);
        case 171:
            return radicalInverseScrambleLoop<1021>(perm, a);
        case 172:
            return radicalInverseScrambleLoop<1031>(perm, a);
        case 173:
            return radicalInverseScrambleLoop<1033>(perm, a);
        case 174:
            return radicalInverseScrambleLoop<1039>(perm, a);
        case 175:
            return radicalInverseScrambleLoop<1049>(perm, a);
        case 176:
            return radicalInverseScrambleLoop<1051>(perm, a);
        case 177:
            return radicalInverseScrambleLoop<1061>(perm, a);
        case 178:
            return radicalInverseScrambleLoop<1063>(perm, a);
        case 179:
            return radicalInverseScrambleLoop<1069>(perm, a);
        case 180:
            return radicalInverseScrambleLoop<1087>(perm, a);
        case 181:
            return radicalInverseScrambleLoop<1091>(perm, a);
        case 182:
            return radicalInverseScrambleLoop<1093>(perm, a);
        case 183:
            return radicalInverseScrambleLoop<1097>(perm, a);
        case 184:
            return radicalInverseScrambleLoop<1103>(perm, a);
        case 185:
            return radicalInverseScrambleLoop<1109>(perm, a);
        case 186:
            return radicalInverseScrambleLoop<1117>(perm, a);
        case 187:
            return radicalInverseScrambleLoop<1123>(perm, a);
        case 188:
            return radicalInverseScrambleLoop<1129>(perm, a);
        case 189:
            return radicalInverseScrambleLoop<1151>(perm, a);
        case 190:
            return radicalInverseScrambleLoop<1153>(perm, a);
        case 191:
            return radicalInverseScrambleLoop<1163>(perm, a);
        case 192:
            return radicalInverseScrambleLoop<1171>(perm, a);
        case 193:
            return radicalInverseScrambleLoop<1181>(perm, a);
        case 194:
            return radicalInverseScrambleLoop<1187>(perm, a);
        case 195:
            return radicalInverseScrambleLoop<1193>(perm, a);
        case 196:
            return radicalInverseScrambleLoop<1201>(perm, a);
        case 197:
            return radicalInverseScrambleLoop<1213>(perm, a);
        case 198:
            return radicalInverseScrambleLoop<1217>(perm, a);
        case 199:
            return radicalInverseScrambleLoop<1223>(perm, a);
        case 200:
            return radicalInverseScrambleLoop<1229>(perm, a);
        case 201:
            return radicalInverseScrambleLoop<1231>(perm, a);
        case 202:
            return radicalInverseScrambleLoop<1237>(perm, a);
        case 203:
            return radicalInverseScrambleLoop<1249>(perm, a);
        case 204:
            return radicalInverseScrambleLoop<1259>(perm, a);
        case 205:
            return radicalInverseScrambleLoop<1277>(perm, a);
        case 206:
            return radicalInverseScrambleLoop<1279>(perm, a);
        case 207:
            return radicalInverseScrambleLoop<1283>(perm, a);
        case 208:
            return radicalInverseScrambleLoop<1289>(perm, a);
        case 209:
            return radicalInverseScrambleLoop<1291>(perm, a);
        case 210:
            return radicalInverseScrambleLoop<1297>(perm, a);
        case 211:
            return radicalInverseScrambleLoop<1301>(perm, a);
        case 212:
            return radicalInverseScrambleLoop<1303>(perm, a);
        case 213:
            return radicalInverseScrambleLoop<1307>(perm, a);
        case 214:
            return radicalInverseScrambleLoop<1319>(perm, a);
        case 215:
            return radicalInverseScrambleLoop<1321>(perm, a);
        case 216:
            return radicalInverseScrambleLoop<1327>(perm, a);
        case 217:
            return radicalInverseScrambleLoop<1361>(perm, a);
        case 218:
            return radicalInverseScrambleLoop<1367>(perm, a);
        case 219:
            return radicalInverseScrambleLoop<1373>(perm, a);
        case 220:
            return radicalInverseScrambleLoop<1381>(perm, a);
        case 221:
            return radicalInverseScrambleLoop<1399>(perm, a);
        case 222:
            return radicalInverseScrambleLoop<1409>(perm, a);
        case 223:
            return radicalInverseScrambleLoop<1423>(perm, a);
        case 224:
            return radicalInverseScrambleLoop<1427>(perm, a);
        case 225:
            return radicalInverseScrambleLoop<1429>(perm, a);
        case 226:
            return radicalInverseScrambleLoop<1433>(perm, a);
        case 227:
            return radicalInverseScrambleLoop<1439>(perm, a);
        case 228:
            return radicalInverseScrambleLoop<1447>(perm, a);
        case 229:
            return radicalInverseScrambleLoop<1451>(perm, a);
        case 230:
            return radicalInverseScrambleLoop<1453>(perm, a);
        case 231:
            return radicalInverseScrambleLoop<1459>(perm, a);
        case 232:
            return radicalInverseScrambleLoop<1471>(perm, a);
        case 233:
            return radicalInverseScrambleLoop<1481>(perm, a);
        case 234:
            return radicalInverseScrambleLoop<1483>(perm, a);
        case 235:
            return radicalInverseScrambleLoop<1487>(perm, a);
        case 236:
            return radicalInverseScrambleLoop<1489>(perm, a);
        case 237:
            return radicalInverseScrambleLoop<1493>(perm, a);
        case 238:
            return radicalInverseScrambleLoop<1499>(perm, a);
        case 239:
            return radicalInverseScrambleLoop<1511>(perm, a);
        case 240:
            return radicalInverseScrambleLoop<1523>(perm, a);
        case 241:
            return radicalInverseScrambleLoop<1531>(perm, a);
        case 242:
            return radicalInverseScrambleLoop<1543>(perm, a);
        case 243:
            return radicalInverseScrambleLoop<1549>(perm, a);
        case 244:
            return radicalInverseScrambleLoop<1553>(perm, a);
        case 245:
            return radicalInverseScrambleLoop<1559>(perm, a);
        case 246:
            return radicalInverseScrambleLoop<1567>(perm, a);
        case 247:
            return radicalInverseScrambleLoop<1571>(perm, a);
        case 248:
            return radicalInverseScrambleLoop<1579>(perm, a);
        case 249:
            return radicalInverseScrambleLoop<1583>(perm, a);
        case 250:
            return radicalInverseScrambleLoop<1597>(perm, a);
        case 251:
            return radicalInverseScrambleLoop<1601>(perm, a);
        case 252:
            return radicalInverseScrambleLoop<1607>(perm, a);
        case 253:
            return radicalInverseScrambleLoop<1609>(perm, a);
        case 254:
            return radicalInverseScrambleLoop<1613>(perm, a);
        case 255:
            return radicalInverseScrambleLoop<1619>(perm, a);
        case 256:
            return radicalInverseScrambleLoop<1621>(perm, a);
        case 257:
            return radicalInverseScrambleLoop<1627>(perm, a);
        case 258:
            return radicalInverseScrambleLoop<1637>(perm, a);
        case 259:
            return radicalInverseScrambleLoop<1657>(perm, a);
        case 260:
            return radicalInverseScrambleLoop<1663>(perm, a);
        case 261:
            return radicalInverseScrambleLoop<1667>(perm, a);
        case 262:
            return radicalInverseScrambleLoop<1669>(perm, a);
        case 263:
            return radicalInverseScrambleLoop<1693>(perm, a);
        case 264:
            return radicalInverseScrambleLoop<1697>(perm, a);
        case 265:
            return radicalInverseScrambleLoop<1699>(perm, a);
        case 266:
            return radicalInverseScrambleLoop<1709>(perm, a);
        case 267:
            return radicalInverseScrambleLoop<1721>(perm, a);
        case 268:
            return radicalInverseScrambleLoop<1723>(perm, a);
        case 269:
            return radicalInverseScrambleLoop<1733>(perm, a);
        case 270:
            return radicalInverseScrambleLoop<1741>(perm, a);
        case 271:
            return radicalInverseScrambleLoop<1747>(perm, a);
        case 272:
            return radicalInverseScrambleLoop<1753>(perm, a);
        case 273:
            return radicalInverseScrambleLoop<1759>(perm, a);
        case 274:
            return radicalInverseScrambleLoop<1777>(perm, a);
        case 275:
            return radicalInverseScrambleLoop<1783>(perm, a);
        case 276:
            return radicalInverseScrambleLoop<1787>(perm, a);
        case 277:
            return radicalInverseScrambleLoop<1789>(perm, a);
        case 278:
            return radicalInverseScrambleLoop<1801>(perm, a);
        case 279:
            return radicalInverseScrambleLoop<1811>(perm, a);
        case 280:
            return radicalInverseScrambleLoop<1823>(perm, a);
        case 281:
            return radicalInverseScrambleLoop<1831>(perm, a);
        case 282:
            return radicalInverseScrambleLoop<1847>(perm, a);
        case 283:
            return radicalInverseScrambleLoop<1861>(perm, a);
        case 284:
            return radicalInverseScrambleLoop<1867>(perm, a);
        case 285:
            return radicalInverseScrambleLoop<1871>(perm, a);
        case 286:
            return radicalInverseScrambleLoop<1873>(perm, a);
        case 287:
            return radicalInverseScrambleLoop<1877>(perm, a);
        case 288:
            return radicalInverseScrambleLoop<1879>(perm, a);
        case 289:
            return radicalInverseScrambleLoop<1889>(perm, a);
        case 290:
            return radicalInverseScrambleLoop<1901>(perm, a);
        case 291:
            return radicalInverseScrambleLoop<1907>(perm, a);
        case 292:
            return radicalInverseScrambleLoop<1913>(perm, a);
        case 293:
            return radicalInverseScrambleLoop<1931>(perm, a);
        case 294:
            return radicalInverseScrambleLoop<1933>(perm, a);
        case 295:
            return radicalInverseScrambleLoop<1949>(perm, a);
        case 296:
            return radicalInverseScrambleLoop<1951>(perm, a);
        case 297:
            return radicalInverseScrambleLoop<1973>(perm, a);
        case 298:
            return radicalInverseScrambleLoop<1979>(perm, a);
        case 299:
            return radicalInverseScrambleLoop<1987>(perm, a);
        case 300:
            return radicalInverseScrambleLoop<1993>(perm, a);
        case 301:
            return radicalInverseScrambleLoop<1997>(perm, a);
        case 302:
            return radicalInverseScrambleLoop<1999>(perm, a);
        case 303:
            return radicalInverseScrambleLoop<2003>(perm, a);
        case 304:
            return radicalInverseScrambleLoop<2011>(perm, a);
        case 305:
            return radicalInverseScrambleLoop<2017>(perm, a);
        case 306:
            return radicalInverseScrambleLoop<2027>(perm, a);
        case 307:
            return radicalInverseScrambleLoop<2029>(perm, a);
        case 308:
            return radicalInverseScrambleLoop<2039>(perm, a);
        case 309:
            return radicalInverseScrambleLoop<2053>(perm, a);
        case 310:
            return radicalInverseScrambleLoop<2063>(perm, a);
        case 311:
            return radicalInverseScrambleLoop<2069>(perm, a);
        case 312:
            return radicalInverseScrambleLoop<2081>(perm, a);
        case 313:
            return radicalInverseScrambleLoop<2083>(perm, a);
        case 314:
            return radicalInverseScrambleLoop<2087>(perm, a);
        case 315:
            return radicalInverseScrambleLoop<2089>(perm, a);
        case 316:
            return radicalInverseScrambleLoop<2099>(perm, a);
        case 317:
            return radicalInverseScrambleLoop<2111>(perm, a);
        case 318:
            return radicalInverseScrambleLoop<2113>(perm, a);
        case 319:
            return radicalInverseScrambleLoop<2129>(perm, a);
        case 320:
            return radicalInverseScrambleLoop<2131>(perm, a);
        case 321:
            return radicalInverseScrambleLoop<2137>(perm, a);
        case 322:
            return radicalInverseScrambleLoop<2141>(perm, a);
        case 323:
            return radicalInverseScrambleLoop<2143>(perm, a);
        case 324:
            return radicalInverseScrambleLoop<2153>(perm, a);
        case 325:
            return radicalInverseScrambleLoop<2161>(perm, a);
        case 326:
            return radicalInverseScrambleLoop<2179>(perm, a);
        case 327:
            return radicalInverseScrambleLoop<2203>(perm, a);
        case 328:
            return radicalInverseScrambleLoop<2207>(perm, a);
        case 329:
            return radicalInverseScrambleLoop<2213>(perm, a);
        case 330:
            return radicalInverseScrambleLoop<2221>(perm, a);
        case 331:
            return radicalInverseScrambleLoop<2237>(perm, a);
        case 332:
            return radicalInverseScrambleLoop<2239>(perm, a);
        case 333:
            return radicalInverseScrambleLoop<2243>(perm, a);
        case 334:
            return radicalInverseScrambleLoop<2251>(perm, a);
        case 335:
            return radicalInverseScrambleLoop<2267>(perm, a);
        case 336:
            return radicalInverseScrambleLoop<2269>(perm, a);
        case 337:
            return radicalInverseScrambleLoop<2273>(perm, a);
        case 338:
            return radicalInverseScrambleLoop<2281>(perm, a);
        case 339:
            return radicalInverseScrambleLoop<2287>(perm, a);
        case 340:
            return radicalInverseScrambleLoop<2293>(perm, a);
        case 341:
            return radicalInverseScrambleLoop<2297>(perm, a);
        case 342:
            return radicalInverseScrambleLoop<2309>(perm, a);
        case 343:
            return radicalInverseScrambleLoop<2311>(perm, a);
        case 344:
            return radicalInverseScrambleLoop<2333>(perm, a);
        case 345:
            return radicalInverseScrambleLoop<2339>(perm, a);
        case 346:
            return radicalInverseScrambleLoop<2341>(perm, a);
        case 347:
            return radicalInverseScrambleLoop<2347>(perm, a);
        case 348:
            return radicalInverseScrambleLoop<2351>(perm, a);
        case 349:
            return radicalInverseScrambleLoop<2357>(perm, a);
        case 350:
            return radicalInverseScrambleLoop<2371>(perm, a);
        case 351:
            return radicalInverseScrambleLoop<2377>(perm, a);
        case 352:
            return radicalInverseScrambleLoop<2381>(perm, a);
        case 353:
            return radicalInverseScrambleLoop<2383>(perm, a);
        case 354:
            return radicalInverseScrambleLoop<2389>(perm, a);
        case 355:
            return radicalInverseScrambleLoop<2393>(perm, a);
        case 356:
            return radicalInverseScrambleLoop<2399>(perm, a);
        case 357:
            return radicalInverseScrambleLoop<2411>(perm, a);
        case 358:
            return radicalInverseScrambleLoop<2417>(perm, a);
        case 359:
            return radicalInverseScrambleLoop<2423>(perm, a);
        case 360:
            return radicalInverseScrambleLoop<2437>(perm, a);
        case 361:
            return radicalInverseScrambleLoop<2441>(perm, a);
        case 362:
            return radicalInverseScrambleLoop<2447>(perm, a);
        case 363:
            return radicalInverseScrambleLoop<2459>(perm, a);
        case 364:
            return radicalInverseScrambleLoop<2467>(perm, a);
        case 365:
            return radicalInverseScrambleLoop<2473>(perm, a);
        case 366:
            return radicalInverseScrambleLoop<2477>(perm, a);
        case 367:
            return radicalInverseScrambleLoop<2503>(perm, a);
        case 368:
            return radicalInverseScrambleLoop<2521>(perm, a);
        case 369:
            return radicalInverseScrambleLoop<2531>(perm, a);
        case 370:
            return radicalInverseScrambleLoop<2539>(perm, a);
        case 371:
            return radicalInverseScrambleLoop<2543>(perm, a);
        case 372:
            return radicalInverseScrambleLoop<2549>(perm, a);
        case 373:
            return radicalInverseScrambleLoop<2551>(perm, a);
        case 374:
            return radicalInverseScrambleLoop<2557>(perm, a);
        case 375:
            return radicalInverseScrambleLoop<2579>(perm, a);
        case 376:
            return radicalInverseScrambleLoop<2591>(perm, a);
        case 377:
            return radicalInverseScrambleLoop<2593>(perm, a);
        case 378:
            return radicalInverseScrambleLoop<2609>(perm, a);
        case 379:
            return radicalInverseScrambleLoop<2617>(perm, a);
        case 380:
            return radicalInverseScrambleLoop<2621>(perm, a);
        case 381:
            return radicalInverseScrambleLoop<2633>(perm, a);
        case 382:
            return radicalInverseScrambleLoop<2647>(perm, a);
        case 383:
            return radicalInverseScrambleLoop<2657>(perm, a);
        case 384:
            return radicalInverseScrambleLoop<2659>(perm, a);
        case 385:
            return radicalInverseScrambleLoop<2663>(perm, a);
        case 386:
            return radicalInverseScrambleLoop<2671>(perm, a);
        case 387:
            return radicalInverseScrambleLoop<2677>(perm, a);
        case 388:
            return radicalInverseScrambleLoop<2683>(perm, a);
        case 389:
            return radicalInverseScrambleLoop<2687>(perm, a);
        case 390:
            return radicalInverseScrambleLoop<2689>(perm, a);
        case 391:
            return radicalInverseScrambleLoop<2693>(perm, a);
        case 392:
            return radicalInverseScrambleLoop<2699>(perm, a);
        case 393:
            return radicalInverseScrambleLoop<2707>(perm, a);
        case 394:
            return radicalInverseScrambleLoop<2711>(perm, a);
        case 395:
            return radicalInverseScrambleLoop<2713>(perm, a);
        case 396:
            return radicalInverseScrambleLoop<2719>(perm, a);
        case 397:
            return radicalInverseScrambleLoop<2729>(perm, a);
        case 398:
            return radicalInverseScrambleLoop<2731>(perm, a);
        case 399:
            return radicalInverseScrambleLoop<2741>(perm, a);
        case 400:
            return radicalInverseScrambleLoop<2749>(perm, a);
        case 401:
            return radicalInverseScrambleLoop<2753>(perm, a);
        case 402:
            return radicalInverseScrambleLoop<2767>(perm, a);
        case 403:
            return radicalInverseScrambleLoop<2777>(perm, a);
        case 404:
            return radicalInverseScrambleLoop<2789>(perm, a);
        case 405:
            return radicalInverseScrambleLoop<2791>(perm, a);
        case 406:
            return radicalInverseScrambleLoop<2797>(perm, a);
        case 407:
            return radicalInverseScrambleLoop<2801>(perm, a);
        case 408:
            return radicalInverseScrambleLoop<2803>(perm, a);
        case 409:
            return radicalInverseScrambleLoop<2819>(perm, a);
        case 410:
            return radicalInverseScrambleLoop<2833>(perm, a);
        case 411:
            return radicalInverseScrambleLoop<2837>(perm, a);
        case 412:
            return radicalInverseScrambleLoop<2843>(perm, a);
        case 413:
            return radicalInverseScrambleLoop<2851>(perm, a);
        case 414:
            return radicalInverseScrambleLoop<2857>(perm, a);
        case 415:
            return radicalInverseScrambleLoop<2861>(perm, a);
        case 416:
            return radicalInverseScrambleLoop<2879>(perm, a);
        case 417:
            return radicalInverseScrambleLoop<2887>(perm, a);
        case 418:
            return radicalInverseScrambleLoop<2897>(perm, a);
        case 419:
            return radicalInverseScrambleLoop<2903>(perm, a);
        case 420:
            return radicalInverseScrambleLoop<2909>(perm, a);
        case 421:
            return radicalInverseScrambleLoop<2917>(perm, a);
        case 422:
            return radicalInverseScrambleLoop<2927>(perm, a);
        case 423:
            return radicalInverseScrambleLoop<2939>(perm, a);
        case 424:
            return radicalInverseScrambleLoop<2953>(perm, a);
        case 425:
            return radicalInverseScrambleLoop<2957>(perm, a);
        case 426:
            return radicalInverseScrambleLoop<2963>(perm, a);
        case 427:
            return radicalInverseScrambleLoop<2969>(perm, a);
        case 428:
            return radicalInverseScrambleLoop<2971>(perm, a);
        case 429:
            return radicalInverseScrambleLoop<2999>(perm, a);
        case 430:
            return radicalInverseScrambleLoop<3001>(perm, a);
        case 431:
            return radicalInverseScrambleLoop<3011>(perm, a);
        case 432:
            return radicalInverseScrambleLoop<3019>(perm, a);
        case 433:
            return radicalInverseScrambleLoop<3023>(perm, a);
        case 434:
            return radicalInverseScrambleLoop<3037>(perm, a);
        case 435:
            return radicalInverseScrambleLoop<3041>(perm, a);
        case 436:
            return radicalInverseScrambleLoop<3049>(perm, a);
        case 437:
            return radicalInverseScrambleLoop<3061>(perm, a);
        case 438:
            return radicalInverseScrambleLoop<3067>(perm, a);
        case 439:
            return radicalInverseScrambleLoop<3079>(perm, a);
        case 440:
            return radicalInverseScrambleLoop<3083>(perm, a);
        case 441:
            return radicalInverseScrambleLoop<3089>(perm, a);
        case 442:
            return radicalInverseScrambleLoop<3109>(perm, a);
        case 443:
            return radicalInverseScrambleLoop<3119>(perm, a);
        case 444:
            return radicalInverseScrambleLoop<3121>(perm, a);
        case 445:
            return radicalInverseScrambleLoop<3137>(perm, a);
        case 446:
            return radicalInverseScrambleLoop<3163>(perm, a);
        case 447:
            return radicalInverseScrambleLoop<3167>(perm, a);
        case 448:
            return radicalInverseScrambleLoop<3169>(perm, a);
        case 449:
            return radicalInverseScrambleLoop<3181>(perm, a);
        case 450:
            return radicalInverseScrambleLoop<3187>(perm, a);
        case 451:
            return radicalInverseScrambleLoop<3191>(perm, a);
        case 452:
            return radicalInverseScrambleLoop<3203>(perm, a);
        case 453:
            return radicalInverseScrambleLoop<3209>(perm, a);
        case 454:
            return radicalInverseScrambleLoop<3217>(perm, a);
        case 455:
            return radicalInverseScrambleLoop<3221>(perm, a);
        case 456:
            return radicalInverseScrambleLoop<3229>(perm, a);
        case 457:
            return radicalInverseScrambleLoop<3251>(perm, a);
        case 458:
            return radicalInverseScrambleLoop<3253>(perm, a);
        case 459:
            return radicalInverseScrambleLoop<3257>(perm, a);
        case 460:
            return radicalInverseScrambleLoop<3259>(perm, a);
        case 461:
            return radicalInverseScrambleLoop<3271>(perm, a);
        case 462:
            return radicalInverseScrambleLoop<3299>(perm, a);
        case 463:
            return radicalInverseScrambleLoop<3301>(perm, a);
        case 464:
            return radicalInverseScrambleLoop<3307>(perm, a);
        case 465:
            return radicalInverseScrambleLoop<3313>(perm, a);
        case 466:
            return radicalInverseScrambleLoop<3319>(perm, a);
        case 467:
            return radicalInverseScrambleLoop<3323>(perm, a);
        case 468:
            return radicalInverseScrambleLoop<3329>(perm, a);
        case 469:
            return radicalInverseScrambleLoop<3331>(perm, a);
        case 470:
            return radicalInverseScrambleLoop<3343>(perm, a);
        case 471:
            return radicalInverseScrambleLoop<3347>(perm, a);
        case 472:
            return radicalInverseScrambleLoop<3359>(perm, a);
        case 473:
            return radicalInverseScrambleLoop<3361>(perm, a);
        case 474:
            return radicalInverseScrambleLoop<3371>(perm, a);
        case 475:
            return radicalInverseScrambleLoop<3373>(perm, a);
        case 476:
            return radicalInverseScrambleLoop<3389>(perm, a);
        case 477:
            return radicalInverseScrambleLoop<3391>(perm, a);
        case 478:
            return radicalInverseScrambleLoop<3407>(perm, a);
        case 479:
            return radicalInverseScrambleLoop<3413>(perm, a);
        case 480:
            return radicalInverseScrambleLoop<3433>(perm, a);
        case 481:
            return radicalInverseScrambleLoop<3449>(perm, a);
        case 482:
            return radicalInverseScrambleLoop<3457>(perm, a);
        case 483:
            return radicalInverseScrambleLoop<3461>(perm, a);
        case 484:
            return radicalInverseScrambleLoop<3463>(perm, a);
        case 485:
            return radicalInverseScrambleLoop<3467>(perm, a);
        case 486:
            return radicalInverseScrambleLoop<3469>(perm, a);
        case 487:
            return radicalInverseScrambleLoop<3491>(perm, a);
        case 488:
            return radicalInverseScrambleLoop<3499>(perm, a);
        case 489:
            return radicalInverseScrambleLoop<3511>(perm, a);
        case 490:
            return radicalInverseScrambleLoop<3517>(perm, a);
        case 491:
            return radicalInverseScrambleLoop<3527>(perm, a);
        case 492:
            return radicalInverseScrambleLoop<3529>(perm, a);
        case 493:
            return radicalInverseScrambleLoop<3533>(perm, a);
        case 494:
            return radicalInverseScrambleLoop<3539>(perm, a);
        case 495:
            return radicalInverseScrambleLoop<3541>(perm, a);
        case 496:
            return radicalInverseScrambleLoop<3547>(perm, a);
        case 497:
            return radicalInverseScrambleLoop<3557>(perm, a);
        case 498:
            return radicalInverseScrambleLoop<3559>(perm, a);
        case 499:
            return radicalInverseScrambleLoop<3571>(perm, a);
        case 500:
            return radicalInverseScrambleLoop<3581>(perm, a);
        case 501:
            return radicalInverseScrambleLoop<3583>(perm, a);
        case 502:
            return radicalInverseScrambleLoop<3593>(perm, a);
        case 503:
            return radicalInverseScrambleLoop<3607>(perm, a);
        case 504:
            return radicalInverseScrambleLoop<3613>(perm, a);
        case 505:
            return radicalInverseScrambleLoop<3617>(perm, a);
        case 506:
            return radicalInverseScrambleLoop<3623>(perm, a);
        case 507:
            return radicalInverseScrambleLoop<3631>(perm, a);
        case 508:
            return radicalInverseScrambleLoop<3637>(perm, a);
        case 509:
            return radicalInverseScrambleLoop<3643>(perm, a);
        case 510:
            return radicalInverseScrambleLoop<3659>(perm, a);
        case 511:
            return radicalInverseScrambleLoop<3671>(perm, a);
        case 512:
            return radicalInverseScrambleLoop<3673>(perm, a);
        case 513:
            return radicalInverseScrambleLoop<3677>(perm, a);
        case 514:
            return radicalInverseScrambleLoop<3691>(perm, a);
        case 515:
            return radicalInverseScrambleLoop<3697>(perm, a);
        case 516:
            return radicalInverseScrambleLoop<3701>(perm, a);
        case 517:
            return radicalInverseScrambleLoop<3709>(perm, a);
        case 518:
            return radicalInverseScrambleLoop<3719>(perm, a);
        case 519:
            return radicalInverseScrambleLoop<3727>(perm, a);
        case 520:
            return radicalInverseScrambleLoop<3733>(perm, a);
        case 521:
            return radicalInverseScrambleLoop<3739>(perm, a);
        case 522:
            return radicalInverseScrambleLoop<3761>(perm, a);
        case 523:
            return radicalInverseScrambleLoop<3767>(perm, a);
        case 524:
            return radicalInverseScrambleLoop<3769>(perm, a);
        case 525:
            return radicalInverseScrambleLoop<3779>(perm, a);
        case 526:
            return radicalInverseScrambleLoop<3793>(perm, a);
        case 527:
            return radicalInverseScrambleLoop<3797>(perm, a);
        case 528:
            return radicalInverseScrambleLoop<3803>(perm, a);
        case 529:
            return radicalInverseScrambleLoop<3821>(perm, a);
        case 530:
            return radicalInverseScrambleLoop<3823>(perm, a);
        case 531:
            return radicalInverseScrambleLoop<3833>(perm, a);
        case 532:
            return radicalInverseScrambleLoop<3847>(perm, a);
        case 533:
            return radicalInverseScrambleLoop<3851>(perm, a);
        case 534:
            return radicalInverseScrambleLoop<3853>(perm, a);
        case 535:
            return radicalInverseScrambleLoop<3863>(perm, a);
        case 536:
            return radicalInverseScrambleLoop<3877>(perm, a);
        case 537:
            return radicalInverseScrambleLoop<3881>(perm, a);
        case 538:
            return radicalInverseScrambleLoop<3889>(perm, a);
        case 539:
            return radicalInverseScrambleLoop<3907>(perm, a);
        case 540:
            return radicalInverseScrambleLoop<3911>(perm, a);
        case 541:
            return radicalInverseScrambleLoop<3917>(perm, a);
        case 542:
            return radicalInverseScrambleLoop<3919>(perm, a);
        case 543:
            return radicalInverseScrambleLoop<3923>(perm, a);
        case 544:
            return radicalInverseScrambleLoop<3929>(perm, a);
        case 545:
            return radicalInverseScrambleLoop<3931>(perm, a);
        case 546:
            return radicalInverseScrambleLoop<3943>(perm, a);
        case 547:
            return radicalInverseScrambleLoop<3947>(perm, a);
        case 548:
            return radicalInverseScrambleLoop<3967>(perm, a);
        case 549:
            return radicalInverseScrambleLoop<3989>(perm, a);
        case 550:
            return radicalInverseScrambleLoop<4001>(perm, a);
        case 551:
            return radicalInverseScrambleLoop<4003>(perm, a);
        case 552:
            return radicalInverseScrambleLoop<4007>(perm, a);
        case 553:
            return radicalInverseScrambleLoop<4013>(perm, a);
        case 554:
            return radicalInverseScrambleLoop<4019>(perm, a);
        case 555:
            return radicalInverseScrambleLoop<4021>(perm, a);
        case 556:
            return radicalInverseScrambleLoop<4027>(perm, a);
        case 557:
            return radicalInverseScrambleLoop<4049>(perm, a);
        case 558:
            return radicalInverseScrambleLoop<4051>(perm, a);
        case 559:
            return radicalInverseScrambleLoop<4057>(perm, a);
        case 560:
            return radicalInverseScrambleLoop<4073>(perm, a);
        case 561:
            return radicalInverseScrambleLoop<4079>(perm, a);
        case 562:
            return radicalInverseScrambleLoop<4091>(perm, a);
        case 563:
            return radicalInverseScrambleLoop<4093>(perm, a);
        case 564:
            return radicalInverseScrambleLoop<4099>(perm, a);
        case 565:
            return radicalInverseScrambleLoop<4111>(perm, a);
        case 566:
            return radicalInverseScrambleLoop<4127>(perm, a);
        case 567:
            return radicalInverseScrambleLoop<4129>(perm, a);
        case 568:
            return radicalInverseScrambleLoop<4133>(perm, a);
        case 569:
            return radicalInverseScrambleLoop<4139>(perm, a);
        case 570:
            return radicalInverseScrambleLoop<4153>(perm, a);
        case 571:
            return radicalInverseScrambleLoop<4157>(perm, a);
        case 572:
            return radicalInverseScrambleLoop<4159>(perm, a);
        case 573:
            return radicalInverseScrambleLoop<4177>(perm, a);
        case 574:
            return radicalInverseScrambleLoop<4201>(perm, a);
        case 575:
            return radicalInverseScrambleLoop<4211>(perm, a);
        case 576:
            return radicalInverseScrambleLoop<4217>(perm, a);
        case 577:
            return radicalInverseScrambleLoop<4219>(perm, a);
        case 578:
            return radicalInverseScrambleLoop<4229>(perm, a);
        case 579:
            return radicalInverseScrambleLoop<4231>(perm, a);
        case 580:
            return radicalInverseScrambleLoop<4241>(perm, a);
        case 581:
            return radicalInverseScrambleLoop<4243>(perm, a);
        case 582:
            return radicalInverseScrambleLoop<4253>(perm, a);
        case 583:
            return radicalInverseScrambleLoop<4259>(perm, a);
        case 584:
            return radicalInverseScrambleLoop<4261>(perm, a);
        case 585:
            return radicalInverseScrambleLoop<4271>(perm, a);
        case 586:
            return radicalInverseScrambleLoop<4273>(perm, a);
        case 587:
            return radicalInverseScrambleLoop<4283>(perm, a);
        case 588:
            return radicalInverseScrambleLoop<4289>(perm, a);
        case 589:
            return radicalInverseScrambleLoop<4297>(perm, a);
        case 590:
            return radicalInverseScrambleLoop<4327>(perm, a);
        case 591:
            return radicalInverseScrambleLoop<4337>(perm, a);
        case 592:
            return radicalInverseScrambleLoop<4339>(perm, a);
        case 593:
            return radicalInverseScrambleLoop<4349>(perm, a);
        case 594:
            return radicalInverseScrambleLoop<4357>(perm, a);
        case 595:
            return radicalInverseScrambleLoop<4363>(perm, a);
        case 596:
            return radicalInverseScrambleLoop<4373>(perm, a);
        case 597:
            return radicalInverseScrambleLoop<4391>(perm, a);
        case 598:
            return radicalInverseScrambleLoop<4397>(perm, a);
        case 599:
            return radicalInverseScrambleLoop<4409>(perm, a);
        case 600:
            return radicalInverseScrambleLoop<4421>(perm, a);
        case 601:
            return radicalInverseScrambleLoop<4423>(perm, a);
        case 602:
            return radicalInverseScrambleLoop<4441>(perm, a);
        case 603:
            return radicalInverseScrambleLoop<4447>(perm, a);
        case 604:
            return radicalInverseScrambleLoop<4451>(perm, a);
        case 605:
            return radicalInverseScrambleLoop<4457>(perm, a);
        case 606:
            return radicalInverseScrambleLoop<4463>(perm, a);
        case 607:
            return radicalInverseScrambleLoop<4481>(perm, a);
        case 608:
            return radicalInverseScrambleLoop<4483>(perm, a);
        case 609:
            return radicalInverseScrambleLoop<4493>(perm, a);
        case 610:
            return radicalInverseScrambleLoop<4507>(perm, a);
        case 611:
            return radicalInverseScrambleLoop<4513>(perm, a);
        case 612:
            return radicalInverseScrambleLoop<4517>(perm, a);
        case 613:
            return radicalInverseScrambleLoop<4519>(perm, a);
        case 614:
            return radicalInverseScrambleLoop<4523>(perm, a);
        case 615:
            return radicalInverseScrambleLoop<4547>(perm, a);
        case 616:
            return radicalInverseScrambleLoop<4549>(perm, a);
        case 617:
            return radicalInverseScrambleLoop<4561>(perm, a);
        case 618:
            return radicalInverseScrambleLoop<4567>(perm, a);
        case 619:
            return radicalInverseScrambleLoop<4583>(perm, a);
        case 620:
            return radicalInverseScrambleLoop<4591>(perm, a);
        case 621:
            return radicalInverseScrambleLoop<4597>(perm, a);
        case 622:
            return radicalInverseScrambleLoop<4603>(perm, a);
        case 623:
            return radicalInverseScrambleLoop<4621>(perm, a);
        case 624:
            return radicalInverseScrambleLoop<4637>(perm, a);
        case 625:
            return radicalInverseScrambleLoop<4639>(perm, a);
        case 626:
            return radicalInverseScrambleLoop<4643>(perm, a);
        case 627:
            return radicalInverseScrambleLoop<4649>(perm, a);
        case 628:
            return radicalInverseScrambleLoop<4651>(perm, a);
        case 629:
            return radicalInverseScrambleLoop<4657>(perm, a);
        case 630:
            return radicalInverseScrambleLoop<4663>(perm, a);
        case 631:
            return radicalInverseScrambleLoop<4673>(perm, a);
        case 632:
            return radicalInverseScrambleLoop<4679>(perm, a);
        case 633:
            return radicalInverseScrambleLoop<4691>(perm, a);
        case 634:
            return radicalInverseScrambleLoop<4703>(perm, a);
        case 635:
            return radicalInverseScrambleLoop<4721>(perm, a);
        case 636:
            return radicalInverseScrambleLoop<4723>(perm, a);
        case 637:
            return radicalInverseScrambleLoop<4729>(perm, a);
        case 638:
            return radicalInverseScrambleLoop<4733>(perm, a);
        case 639:
            return radicalInverseScrambleLoop<4751>(perm, a);
        case 640:
            return radicalInverseScrambleLoop<4759>(perm, a);
        case 641:
            return radicalInverseScrambleLoop<4783>(perm, a);
        case 642:
            return radicalInverseScrambleLoop<4787>(perm, a);
        case 643:
            return radicalInverseScrambleLoop<4789>(perm, a);
        case 644:
            return radicalInverseScrambleLoop<4793>(perm, a);
        case 645:
            return radicalInverseScrambleLoop<4799>(perm, a);
        case 646:
            return radicalInverseScrambleLoop<4801>(perm, a);
        case 647:
            return radicalInverseScrambleLoop<4813>(perm, a);
        case 648:
            return radicalInverseScrambleLoop<4817>(perm, a);
        case 649:
            return radicalInverseScrambleLoop<4831>(perm, a);
        case 650:
            return radicalInverseScrambleLoop<4861>(perm, a);
        case 651:
            return radicalInverseScrambleLoop<4871>(perm, a);
        case 652:
            return radicalInverseScrambleLoop<4877>(perm, a);
        case 653:
            return radicalInverseScrambleLoop<4889>(perm, a);
        case 654:
            return radicalInverseScrambleLoop<4903>(perm, a);
        case 655:
            return radicalInverseScrambleLoop<4909>(perm, a);
        case 656:
            return radicalInverseScrambleLoop<4919>(perm, a);
        case 657:
            return radicalInverseScrambleLoop<4931>(perm, a);
        case 658:
            return radicalInverseScrambleLoop<4933>(perm, a);
        case 659:
            return radicalInverseScrambleLoop<4937>(perm, a);
        case 660:
            return radicalInverseScrambleLoop<4943>(perm, a);
        case 661:
            return radicalInverseScrambleLoop<4951>(perm, a);
        case 662:
            return radicalInverseScrambleLoop<4957>(perm, a);
        case 663:
            return radicalInverseScrambleLoop<4967>(perm, a);
        case 664:
            return radicalInverseScrambleLoop<4969>(perm, a);
        case 665:
            return radicalInverseScrambleLoop<4973>(perm, a);
        case 666:
            return radicalInverseScrambleLoop<4987>(perm, a);
        case 667:
            return radicalInverseScrambleLoop<4993>(perm, a);
        case 668:
            return radicalInverseScrambleLoop<4999>(perm, a);
        case 669:
            return radicalInverseScrambleLoop<5003>(perm, a);
        case 670:
            return radicalInverseScrambleLoop<5009>(perm, a);
        case 671:
            return radicalInverseScrambleLoop<5011>(perm, a);
        case 672:
            return radicalInverseScrambleLoop<5021>(perm, a);
        case 673:
            return radicalInverseScrambleLoop<5023>(perm, a);
        case 674:
            return radicalInverseScrambleLoop<5039>(perm, a);
        case 675:
            return radicalInverseScrambleLoop<5051>(perm, a);
        case 676:
            return radicalInverseScrambleLoop<5059>(perm, a);
        case 677:
            return radicalInverseScrambleLoop<5077>(perm, a);
        case 678:
            return radicalInverseScrambleLoop<5081>(perm, a);
        case 679:
            return radicalInverseScrambleLoop<5087>(perm, a);
        case 680:
            return radicalInverseScrambleLoop<5099>(perm, a);
        case 681:
            return radicalInverseScrambleLoop<5101>(perm, a);
        case 682:
            return radicalInverseScrambleLoop<5107>(perm, a);
        case 683:
            return radicalInverseScrambleLoop<5113>(perm, a);
        case 684:
            return radicalInverseScrambleLoop<5119>(perm, a);
        case 685:
            return radicalInverseScrambleLoop<5147>(perm, a);
        case 686:
            return radicalInverseScrambleLoop<5153>(perm, a);
        case 687:
            return radicalInverseScrambleLoop<5167>(perm, a);
        case 688:
            return radicalInverseScrambleLoop<5171>(perm, a);
        case 689:
            return radicalInverseScrambleLoop<5179>(perm, a);
        case 690:
            return radicalInverseScrambleLoop<5189>(perm, a);
        case 691:
            return radicalInverseScrambleLoop<5197>(perm, a);
        case 692:
            return radicalInverseScrambleLoop<5209>(perm, a);
        case 693:
            return radicalInverseScrambleLoop<5227>(perm, a);
        case 694:
            return radicalInverseScrambleLoop<5231>(perm, a);
        case 695:
            return radicalInverseScrambleLoop<5233>(perm, a);
        case 696:
            return radicalInverseScrambleLoop<5237>(perm, a);
        case 697:
            return radicalInverseScrambleLoop<5261>(perm, a);
        case 698:
            return radicalInverseScrambleLoop<5273>(perm, a);
        case 699:
            return radicalInverseScrambleLoop<5279>(perm, a);
        case 700:
            return radicalInverseScrambleLoop<5281>(perm, a);
        case 701:
            return radicalInverseScrambleLoop<5297>(perm, a);
        case 702:
            return radicalInverseScrambleLoop<5303>(perm, a);
        case 703:
            return radicalInverseScrambleLoop<5309>(perm, a);
        case 704:
            return radicalInverseScrambleLoop<5323>(perm, a);
        case 705:
            return radicalInverseScrambleLoop<5333>(perm, a);
        case 706:
            return radicalInverseScrambleLoop<5347>(perm, a);
        case 707:
            return radicalInverseScrambleLoop<5351>(perm, a);
        case 708:
            return radicalInverseScrambleLoop<5381>(perm, a);
        case 709:
            return radicalInverseScrambleLoop<5387>(perm, a);
        case 710:
            return radicalInverseScrambleLoop<5393>(perm, a);
        case 711:
            return radicalInverseScrambleLoop<5399>(perm, a);
        case 712:
            return radicalInverseScrambleLoop<5407>(perm, a);
        case 713:
            return radicalInverseScrambleLoop<5413>(perm, a);
        case 714:
            return radicalInverseScrambleLoop<5417>(perm, a);
        case 715:
            return radicalInverseScrambleLoop<5419>(perm, a);
        case 716:
            return radicalInverseScrambleLoop<5431>(perm, a);
        case 717:
            return radicalInverseScrambleLoop<5437>(perm, a);
        case 718:
            return radicalInverseScrambleLoop<5441>(perm, a);
        case 719:
            return radicalInverseScrambleLoop<5443>(perm, a);
        case 720:
            return radicalInverseScrambleLoop<5449>(perm, a);
        case 721:
            return radicalInverseScrambleLoop<5471>(perm, a);
        case 722:
            return radicalInverseScrambleLoop<5477>(perm, a);
        case 723:
            return radicalInverseScrambleLoop<5479>(perm, a);
        case 724:
            return radicalInverseScrambleLoop<5483>(perm, a);
        case 725:
            return radicalInverseScrambleLoop<5501>(perm, a);
        case 726:
            return radicalInverseScrambleLoop<5503>(perm, a);
        case 727:
            return radicalInverseScrambleLoop<5507>(perm, a);
        case 728:
            return radicalInverseScrambleLoop<5519>(perm, a);
        case 729:
            return radicalInverseScrambleLoop<5521>(perm, a);
        case 730:
            return radicalInverseScrambleLoop<5527>(perm, a);
        case 731:
            return radicalInverseScrambleLoop<5531>(perm, a);
        case 732:
            return radicalInverseScrambleLoop<5557>(perm, a);
        case 733:
            return radicalInverseScrambleLoop<5563>(perm, a);
        case 734:
            return radicalInverseScrambleLoop<5569>(perm, a);
        case 735:
            return radicalInverseScrambleLoop<5573>(perm, a);
        case 736:
            return radicalInverseScrambleLoop<5581>(perm, a);
        case 737:
            return radicalInverseScrambleLoop<5591>(perm, a);
        case 738:
            return radicalInverseScrambleLoop<5623>(perm, a);
        case 739:
            return radicalInverseScrambleLoop<5639>(perm, a);
        case 740:
            return radicalInverseScrambleLoop<5641>(perm, a);
        case 741:
            return radicalInverseScrambleLoop<5647>(perm, a);
        case 742:
            return radicalInverseScrambleLoop<5651>(perm, a);
        case 743:
            return radicalInverseScrambleLoop<5653>(perm, a);
        case 744:
            return radicalInverseScrambleLoop<5657>(perm, a);
        case 745:
            return radicalInverseScrambleLoop<5659>(perm, a);
        case 746:
            return radicalInverseScrambleLoop<5669>(perm, a);
        case 747:
            return radicalInverseScrambleLoop<5683>(perm, a);
        case 748:
            return radicalInverseScrambleLoop<5689>(perm, a);
        case 749:
            return radicalInverseScrambleLoop<5693>(perm, a);
        case 750:
            return radicalInverseScrambleLoop<5701>(perm, a);
        case 751:
            return radicalInverseScrambleLoop<5711>(perm, a);
        case 752:
            return radicalInverseScrambleLoop<5717>(perm, a);
        case 753:
            return radicalInverseScrambleLoop<5737>(perm, a);
        case 754:
            return radicalInverseScrambleLoop<5741>(perm, a);
        case 755:
            return radicalInverseScrambleLoop<5743>(perm, a);
        case 756:
            return radicalInverseScrambleLoop<5749>(perm, a);
        case 757:
            return radicalInverseScrambleLoop<5779>(perm, a);
        case 758:
            return radicalInverseScrambleLoop<5783>(perm, a);
        case 759:
            return radicalInverseScrambleLoop<5791>(perm, a);
        case 760:
            return radicalInverseScrambleLoop<5801>(perm, a);
        case 761:
            return radicalInverseScrambleLoop<5807>(perm, a);
        case 762:
            return radicalInverseScrambleLoop<5813>(perm, a);
        case 763:
            return radicalInverseScrambleLoop<5821>(perm, a);
        case 764:
            return radicalInverseScrambleLoop<5827>(perm, a);
        case 765:
            return radicalInverseScrambleLoop<5839>(perm, a);
        case 766:
            return radicalInverseScrambleLoop<5843>(perm, a);
        case 767:
            return radicalInverseScrambleLoop<5849>(perm, a);
        case 768:
            return radicalInverseScrambleLoop<5851>(perm, a);
        case 769:
            return radicalInverseScrambleLoop<5857>(perm, a);
        case 770:
            return radicalInverseScrambleLoop<5861>(perm, a);
        case 771:
            return radicalInverseScrambleLoop<5867>(perm, a);
        case 772:
            return radicalInverseScrambleLoop<5869>(perm, a);
        case 773:
            return radicalInverseScrambleLoop<5879>(perm, a);
        case 774:
            return radicalInverseScrambleLoop<5881>(perm, a);
        case 775:
            return radicalInverseScrambleLoop<5897>(perm, a);
        case 776:
            return radicalInverseScrambleLoop<5903>(perm, a);
        case 777:
            return radicalInverseScrambleLoop<5923>(perm, a);
        case 778:
            return radicalInverseScrambleLoop<5927>(perm, a);
        case 779:
            return radicalInverseScrambleLoop<5939>(perm, a);
        case 780:
            return radicalInverseScrambleLoop<5953>(perm, a);
        case 781:
            return radicalInverseScrambleLoop<5981>(perm, a);
        case 782:
            return radicalInverseScrambleLoop<5987>(perm, a);
        case 783:
            return radicalInverseScrambleLoop<6007>(perm, a);
        case 784:
            return radicalInverseScrambleLoop<6011>(perm, a);
        case 785:
            return radicalInverseScrambleLoop<6029>(perm, a);
        case 786:
            return radicalInverseScrambleLoop<6037>(perm, a);
        case 787:
            return radicalInverseScrambleLoop<6043>(perm, a);
        case 788:
            return radicalInverseScrambleLoop<6047>(perm, a);
        case 789:
            return radicalInverseScrambleLoop<6053>(perm, a);
        case 790:
            return radicalInverseScrambleLoop<6067>(perm, a);
        case 791:
            return radicalInverseScrambleLoop<6073>(perm, a);
        case 792:
            return radicalInverseScrambleLoop<6079>(perm, a);
        case 793:
            return radicalInverseScrambleLoop<6089>(perm, a);
        case 794:
            return radicalInverseScrambleLoop<6091>(perm, a);
        case 795:
            return radicalInverseScrambleLoop<6101>(perm, a);
        case 796:
            return radicalInverseScrambleLoop<6113>(perm, a);
        case 797:
            return radicalInverseScrambleLoop<6121>(perm, a);
        case 798:
            return radicalInverseScrambleLoop<6131>(perm, a);
        case 799:
            return radicalInverseScrambleLoop<6133>(perm, a);
        case 800:
            return radicalInverseScrambleLoop<6143>(perm, a);
        case 801:
            return radicalInverseScrambleLoop<6151>(perm, a);
        case 802:
            return radicalInverseScrambleLoop<6163>(perm, a);
        case 803:
            return radicalInverseScrambleLoop<6173>(perm, a);
        case 804:
            return radicalInverseScrambleLoop<6197>(perm, a);
        case 805:
            return radicalInverseScrambleLoop<6199>(perm, a);
        case 806:
            return radicalInverseScrambleLoop<6203>(perm, a);
        case 807:
            return radicalInverseScrambleLoop<6211>(perm, a);
        case 808:
            return radicalInverseScrambleLoop<6217>(perm, a);
        case 809:
            return radicalInverseScrambleLoop<6221>(perm, a);
        case 810:
            return radicalInverseScrambleLoop<6229>(perm, a);
        case 811:
            return radicalInverseScrambleLoop<6247>(perm, a);
        case 812:
            return radicalInverseScrambleLoop<6257>(perm, a);
        case 813:
            return radicalInverseScrambleLoop<6263>(perm, a);
        case 814:
            return radicalInverseScrambleLoop<6269>(perm, a);
        case 815:
            return radicalInverseScrambleLoop<6271>(perm, a);
        case 816:
            return radicalInverseScrambleLoop<6277>(perm, a);
        case 817:
            return radicalInverseScrambleLoop<6287>(perm, a);
        case 818:
            return radicalInverseScrambleLoop<6299>(perm, a);
        case 819:
            return radicalInverseScrambleLoop<6301>(perm, a);
        case 820:
            return radicalInverseScrambleLoop<6311>(perm, a);
        case 821:
            return radicalInverseScrambleLoop<6317>(perm, a);
        case 822:
            return radicalInverseScrambleLoop<6323>(perm, a);
        case 823:
            return radicalInverseScrambleLoop<6329>(perm, a);
        case 824:
            return radicalInverseScrambleLoop<6337>(perm, a);
        case 825:
            return radicalInverseScrambleLoop<6343>(perm, a);
        case 826:
            return radicalInverseScrambleLoop<6353>(perm, a);
        case 827:
            return radicalInverseScrambleLoop<6359>(perm, a);
        case 828:
            return radicalInverseScrambleLoop<6361>(perm, a);
        case 829:
            return radicalInverseScrambleLoop<6367>(perm, a);
        case 830:
            return radicalInverseScrambleLoop<6373>(perm, a);
        case 831:
            return radicalInverseScrambleLoop<6379>(perm, a);
        case 832:
            return radicalInverseScrambleLoop<6389>(perm, a);
        case 833:
            return radicalInverseScrambleLoop<6397>(perm, a);
        case 834:
            return radicalInverseScrambleLoop<6421>(perm, a);
        case 835:
            return radicalInverseScrambleLoop<6427>(perm, a);
        case 836:
            return radicalInverseScrambleLoop<6449>(perm, a);
        case 837:
            return radicalInverseScrambleLoop<6451>(perm, a);
        case 838:
            return radicalInverseScrambleLoop<6469>(perm, a);
        case 839:
            return radicalInverseScrambleLoop<6473>(perm, a);
        case 840:
            return radicalInverseScrambleLoop<6481>(perm, a);
        case 841:
            return radicalInverseScrambleLoop<6491>(perm, a);
        case 842:
            return radicalInverseScrambleLoop<6521>(perm, a);
        case 843:
            return radicalInverseScrambleLoop<6529>(perm, a);
        case 844:
            return radicalInverseScrambleLoop<6547>(perm, a);
        case 845:
            return radicalInverseScrambleLoop<6551>(perm, a);
        case 846:
            return radicalInverseScrambleLoop<6553>(perm, a);
        case 847:
            return radicalInverseScrambleLoop<6563>(perm, a);
        case 848:
            return radicalInverseScrambleLoop<6569>(perm, a);
        case 849:
            return radicalInverseScrambleLoop<6571>(perm, a);
        case 850:
            return radicalInverseScrambleLoop<6577>(perm, a);
        case 851:
            return radicalInverseScrambleLoop<6581>(perm, a);
        case 852:
            return radicalInverseScrambleLoop<6599>(perm, a);
        case 853:
            return radicalInverseScrambleLoop<6607>(perm, a);
        case 854:
            return radicalInverseScrambleLoop<6619>(perm, a);
        case 855:
            return radicalInverseScrambleLoop<6637>(perm, a);
        case 856:
            return radicalInverseScrambleLoop<6653>(perm, a);
        case 857:
            return radicalInverseScrambleLoop<6659>(perm, a);
        case 858:
            return radicalInverseScrambleLoop<6661>(perm, a);
        case 859:
            return radicalInverseScrambleLoop<6673>(perm, a);
        case 860:
            return radicalInverseScrambleLoop<6679>(perm, a);
        case 861:
            return radicalInverseScrambleLoop<6689>(perm, a);
        case 862:
            return radicalInverseScrambleLoop<6691>(perm, a);
        case 863:
            return radicalInverseScrambleLoop<6701>(perm, a);
        case 864:
            return radicalInverseScrambleLoop<6703>(perm, a);
        case 865:
            return radicalInverseScrambleLoop<6709>(perm, a);
        case 866:
            return radicalInverseScrambleLoop<6719>(perm, a);
        case 867:
            return radicalInverseScrambleLoop<6733>(perm, a);
        case 868:
            return radicalInverseScrambleLoop<6737>(perm, a);
        case 869:
            return radicalInverseScrambleLoop<6761>(perm, a);
        case 870:
            return radicalInverseScrambleLoop<6763>(perm, a);
        case 871:
            return radicalInverseScrambleLoop<6779>(perm, a);
        case 872:
            return radicalInverseScrambleLoop<6781>(perm, a);
        case 873:
            return radicalInverseScrambleLoop<6791>(perm, a);
        case 874:
            return radicalInverseScrambleLoop<6793>(perm, a);
        case 875:
            return radicalInverseScrambleLoop<6803>(perm, a);
        case 876:
            return radicalInverseScrambleLoop<6823>(perm, a);
        case 877:
            return radicalInverseScrambleLoop<6827>(perm, a);
        case 878:
            return radicalInverseScrambleLoop<6829>(perm, a);
        case 879:
            return radicalInverseScrambleLoop<6833>(perm, a);
        case 880:
            return radicalInverseScrambleLoop<6841>(perm, a);
        case 881:
            return radicalInverseScrambleLoop<6857>(perm, a);
        case 882:
            return radicalInverseScrambleLoop<6863>(perm, a);
        case 883:
            return radicalInverseScrambleLoop<6869>(perm, a);
        case 884:
            return radicalInverseScrambleLoop<6871>(perm, a);
        case 885:
            return radicalInverseScrambleLoop<6883>(perm, a);
        case 886:
            return radicalInverseScrambleLoop<6899>(perm, a);
        case 887:
            return radicalInverseScrambleLoop<6907>(perm, a);
        case 888:
            return radicalInverseScrambleLoop<6911>(perm, a);
        case 889:
            return radicalInverseScrambleLoop<6917>(perm, a);
        case 890:
            return radicalInverseScrambleLoop<6947>(perm, a);
        case 891:
            return radicalInverseScrambleLoop<6949>(perm, a);
        case 892:
            return radicalInverseScrambleLoop<6959>(perm, a);
        case 893:
            return radicalInverseScrambleLoop<6961>(perm, a);
        case 894:
            return radicalInverseScrambleLoop<6967>(perm, a);
        case 895:
            return radicalInverseScrambleLoop<6971>(perm, a);
        case 896:
            return radicalInverseScrambleLoop<6977>(perm, a);
        case 897:
            return radicalInverseScrambleLoop<6983>(perm, a);
        case 898:
            return radicalInverseScrambleLoop<6991>(perm, a);
        case 899:
            return radicalInverseScrambleLoop<6997>(perm, a);
        case 900:
            return radicalInverseScrambleLoop<7001>(perm, a);
        case 901:
            return radicalInverseScrambleLoop<7013>(perm, a);
        case 902:
            return radicalInverseScrambleLoop<7019>(perm, a);
        case 903:
            return radicalInverseScrambleLoop<7027>(perm, a);
        case 904:
            return radicalInverseScrambleLoop<7039>(perm, a);
        case 905:
            return radicalInverseScrambleLoop<7043>(perm, a);
        case 906:
            return radicalInverseScrambleLoop<7057>(perm, a);
        case 907:
            return radicalInverseScrambleLoop<7069>(perm, a);
        case 908:
            return radicalInverseScrambleLoop<7079>(perm, a);
        case 909:
            return radicalInverseScrambleLoop<7103>(perm, a);
        case 910:
            return radicalInverseScrambleLoop<7109>(perm, a);
        case 911:
            return radicalInverseScrambleLoop<7121>(perm, a);
        case 912:
            return radicalInverseScrambleLoop<7127>(perm, a);
        case 913:
            return radicalInverseScrambleLoop<7129>(perm, a);
        case 914:
            return radicalInverseScrambleLoop<7151>(perm, a);
        case 915:
            return radicalInverseScrambleLoop<7159>(perm, a);
        case 916:
            return radicalInverseScrambleLoop<7177>(perm, a);
        case 917:
            return radicalInverseScrambleLoop<7187>(perm, a);
        case 918:
            return radicalInverseScrambleLoop<7193>(perm, a);
        case 919:
            return radicalInverseScrambleLoop<7207>(perm, a);
        case 920:
            return radicalInverseScrambleLoop<7211>(perm, a);
        case 921:
            return radicalInverseScrambleLoop<7213>(perm, a);
        case 922:
            return radicalInverseScrambleLoop<7219>(perm, a);
        case 923:
            return radicalInverseScrambleLoop<7229>(perm, a);
        case 924:
            return radicalInverseScrambleLoop<7237>(perm, a);
        case 925:
            return radicalInverseScrambleLoop<7243>(perm, a);
        case 926:
            return radicalInverseScrambleLoop<7247>(perm, a);
        case 927:
            return radicalInverseScrambleLoop<7253>(perm, a);
        case 928:
            return radicalInverseScrambleLoop<7283>(perm, a);
        case 929:
            return radicalInverseScrambleLoop<7297>(perm, a);
        case 930:
            return radicalInverseScrambleLoop<7307>(perm, a);
        case 931:
            return radicalInverseScrambleLoop<7309>(perm, a);
        case 932:
            return radicalInverseScrambleLoop<7321>(perm, a);
        case 933:
            return radicalInverseScrambleLoop<7331>(perm, a);
        case 934:
            return radicalInverseScrambleLoop<7333>(perm, a);
        case 935:
            return radicalInverseScrambleLoop<7349>(perm, a);
        case 936:
            return radicalInverseScrambleLoop<7351>(perm, a);
        case 937:
            return radicalInverseScrambleLoop<7369>(perm, a);
        case 938:
            return radicalInverseScrambleLoop<7393>(perm, a);
        case 939:
            return radicalInverseScrambleLoop<7411>(perm, a);
        case 940:
            return radicalInverseScrambleLoop<7417>(perm, a);
        case 941:
            return radicalInverseScrambleLoop<7433>(perm, a);
        case 942:
            return radicalInverseScrambleLoop<7451>(perm, a);
        case 943:
            return radicalInverseScrambleLoop<7457>(perm, a);
        case 944:
            return radicalInverseScrambleLoop<7459>(perm, a);
        case 945:
            return radicalInverseScrambleLoop<7477>(perm, a);
        case 946:
            return radicalInverseScrambleLoop<7481>(perm, a);
        case 947:
            return radicalInverseScrambleLoop<7487>(perm, a);
        case 948:
            return radicalInverseScrambleLoop<7489>(perm, a);
        case 949:
            return radicalInverseScrambleLoop<7499>(perm, a);
        case 950:
            return radicalInverseScrambleLoop<7507>(perm, a);
        case 951:
            return radicalInverseScrambleLoop<7517>(perm, a);
        case 952:
            return radicalInverseScrambleLoop<7523>(perm, a);
        case 953:
            return radicalInverseScrambleLoop<7529>(perm, a);
        case 954:
            return radicalInverseScrambleLoop<7537>(perm, a);
        case 955:
            return radicalInverseScrambleLoop<7541>(perm, a);
        case 956:
            return radicalInverseScrambleLoop<7547>(perm, a);
        case 957:
            return radicalInverseScrambleLoop<7549>(perm, a);
        case 958:
            return radicalInverseScrambleLoop<7559>(perm, a);
        case 959:
            return radicalInverseScrambleLoop<7561>(perm, a);
        case 960:
            return radicalInverseScrambleLoop<7573>(perm, a);
        case 961:
            return radicalInverseScrambleLoop<7577>(perm, a);
        case 962:
            return radicalInverseScrambleLoop<7583>(perm, a);
        case 963:
            return radicalInverseScrambleLoop<7589>(perm, a);
        case 964:
            return radicalInverseScrambleLoop<7591>(perm, a);
        case 965:
            return radicalInverseScrambleLoop<7603>(perm, a);
        case 966:
            return radicalInverseScrambleLoop<7607>(perm, a);
        case 967:
            return radicalInverseScrambleLoop<7621>(perm, a);
        case 968:
            return radicalInverseScrambleLoop<7639>(perm, a);
        case 969:
            return radicalInverseScrambleLoop<7643>(perm, a);
        case 970:
            return radicalInverseScrambleLoop<7649>(perm, a);
        case 971:
            return radicalInverseScrambleLoop<7669>(perm, a);
        case 972:
            return radicalInverseScrambleLoop<7673>(perm, a);
        case 973:
            return radicalInverseScrambleLoop<7681>(perm, a);
        case 974:
            return radicalInverseScrambleLoop<7687>(perm, a);
        case 975:
            return radicalInverseScrambleLoop<7691>(perm, a);
        case 976:
            return radicalInverseScrambleLoop<7699>(perm, a);
        case 977:
            return radicalInverseScrambleLoop<7703>(perm, a);
        case 978:
            return radicalInverseScrambleLoop<7717>(perm, a);
        case 979:
            return radicalInverseScrambleLoop<7723>(perm, a);
        case 980:
            return radicalInverseScrambleLoop<7727>(perm, a);
        case 981:
            return radicalInverseScrambleLoop<7741>(perm, a);
        case 982:
            return radicalInverseScrambleLoop<7753>(perm, a);
        case 983:
            return radicalInverseScrambleLoop<7757>(perm, a);
        case 984:
            return radicalInverseScrambleLoop<7759>(perm, a);
        case 985:
            return radicalInverseScrambleLoop<7789>(perm, a);
        case 986:
            return radicalInverseScrambleLoop<7793>(perm, a);
        case 987:
            return radicalInverseScrambleLoop<7817>(perm, a);
        case 988:
            return radicalInverseScrambleLoop<7823>(perm, a);
        case 989:
            return radicalInverseScrambleLoop<7829>(perm, a);
        case 990:
            return radicalInverseScrambleLoop<7841>(perm, a);
        case 991:
            return radicalInverseScrambleLoop<7853>(perm, a);
        case 992:
            return radicalInverseScrambleLoop<7867>(perm, a);
        case 993:
            return radicalInverseScrambleLoop<7873>(perm, a);
        case 994:
            return radicalInverseScrambleLoop<7877>(perm, a);
        case 995:
            return radicalInverseScrambleLoop<7879>(perm, a);
        case 996:
            return radicalInverseScrambleLoop<7883>(perm, a);
        case 997:
            return radicalInverseScrambleLoop<7901>(perm, a);
        case 998:
            return radicalInverseScrambleLoop<7907>(perm, a);
        case 999:
            return radicalInverseScrambleLoop<7919>(perm, a);
        case 1000:
            return radicalInverseScrambleLoop<7927>(perm, a);
        case 1001:
            return radicalInverseScrambleLoop<7933>(perm, a);
        case 1002:
            return radicalInverseScrambleLoop<7937>(perm, a);
        case 1003:
            return radicalInverseScrambleLoop<7949>(perm, a);
        case 1004:
            return radicalInverseScrambleLoop<7951>(perm, a);
        case 1005:
            return radicalInverseScrambleLoop<7963>(perm, a);
        case 1006:
            return radicalInverseScrambleLoop<7993>(perm, a);
        case 1007:
            return radicalInverseScrambleLoop<8009>(perm, a);
        case 1008:
            return radicalInverseScrambleLoop<8011>(perm, a);
        case 1009:
            return radicalInverseScrambleLoop<8017>(perm, a);
        case 1010:
            return radicalInverseScrambleLoop<8039>(perm, a);
        case 1011:
            return radicalInverseScrambleLoop<8053>(perm, a);
        case 1012:
            return radicalInverseScrambleLoop<8059>(perm, a);
        case 1013:
            return radicalInverseScrambleLoop<8069>(perm, a);
        case 1014:
            return radicalInverseScrambleLoop<8081>(perm, a);
        case 1015:
            return radicalInverseScrambleLoop<8087>(perm, a);
        case 1016:
            return radicalInverseScrambleLoop<8089>(perm, a);
        case 1017:
            return radicalInverseScrambleLoop<8093>(perm, a);
        case 1018:
            return radicalInverseScrambleLoop<8101>(perm, a);
        case 1019:
            return radicalInverseScrambleLoop<8111>(perm, a);
        case 1020:
            return radicalInverseScrambleLoop<8117>(perm, a);
        case 1021:
            return radicalInverseScrambleLoop<8123>(perm, a);
        case 1022:
            return radicalInverseScrambleLoop<8147>(perm, a);
        case 1023:
            return radicalInverseScrambleLoop<8161>(perm, a);
    }
    return 0;
}