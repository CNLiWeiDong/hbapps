
挂单最小金额
1 u

分段列表 使用配置文件，方便配置
```
id	seg_id		sale_price		buy_price	sale_number
1	id2u		2				1.9				100
2	id2.1u		2.1				2.0				100
3	id3u		2.2				2.2				100
```

卖出交易列表 sale_tx
#存在等待的交易，其他任何策略不能执行。不间断查询状态，5分钟未完成，则撤销。
id  	seg_id		order_id	sale_number    sale_price	deal_number		deal_price 	 deal_amount	used_buy_amount	free_buy_amount	create_time	update_time	status 
```

```

# 买入交易列表 buy_tx
# 存在等待的交易，其他任何策略不能执行。不间断查询状态，5分钟未完成，则撤销。
id  sale_tx_id	seg_id	order_id	buy_number    buy_price		deal_number		deal_price 	 deal_amount	status	
```

```

# 买入列表_history	
```

```

# 卖出列表_history	
```

```

