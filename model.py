def match():
    buy = {0: 100, 11000: 500, 10500 :400, 10000 :300}

    sell = {0: 1000,12000: 300, 11500:400, 11000:300}

    traded = 0
    count = 0
    while True:
        sellprice = min(sell.keys())
        if 0 in buy.keys():
            buyprice = 0
        else:
            buyprice = max(buy.keys())
        
        if buyprice == 0 or sellprice <= buyprice:
            diff = sell[sellprice] - buy[buyprice]
            traded += min([sell[sellprice], buy[buyprice]])

            if diff > 0:
                sell[sellprice] = diff
                buy.pop(buyprice)
            elif diff < 0 :
                buy[buyprice] = abs(diff)
                sell.pop(sellprice)
            else:
                sell.pop(sellprice)
                buy.pop(buyprice)
                
            #print(str(buyprice)+ "  " + str(sellprice))
        else:
            break
        count += 1

    return [traded, buy, sell, min(sell.keys())]

x = match()
print(x[0])
print(x[1])
print(x[2])
print(x[3])




