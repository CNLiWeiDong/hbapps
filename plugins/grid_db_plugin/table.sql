/*
卖出交易列表 sale_tx
#存在等待的交易，其他任何策略不能执行。不间断查询状态，5分钟未完成，则撤销。
id  	seg_id		order_id	sale_number    sale_price	deal_number		deal_price 	 deal_amount	used_buy_amount	free_buy_amount	create_time	update_time	status 
*/

CREATE TABLE IF NOT EXISTS sale_tx (
    id           		INT UNSIGNED     NOT NULL AUTO_INCREMENT PRIMARY KEY,
    seg_id      		VARCHAR(255)     NOT NULL,
    seg_price			decimal(20,8)	 NOT NULL,
    order_id      		VARCHAR(255)     NOT NULL,
    sale_number			decimal(20,8)	 NOT NULL,
    sale_price			decimal(20,8)	 NOT NULL,
    deal_number			decimal(20,8)	 NOT NULL DEFAULT 0,
    deal_price			decimal(20,8)	 NOT NULL DEFAULT 0,
    deal_fee			decimal(20,8)	 NOT NULL DEFAULT 0,
    deal_amount			decimal(20,8)	 NOT NULL DEFAULT 0 COMMENT 'used_buy_amount+free_buy_amount+deal_fee=deal_amount',
    used_buy_amount		decimal(20,8)	 NOT NULL DEFAULT 0,	
    free_buy_amount		decimal(20,8)	 NOT NULL DEFAULT 0 ,
    valid_sale_number	decimal(20,8)	 NOT NULL DEFAULT 0 ,
    create_time			TIMESTAMP        NOT NULL DEFAULT CURRENT_TIMESTAMP,
    update_time			TIMESTAMP        NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    status				INT(10)			 NOT NULL DEFAULT 0 COMMENT '(0进行中,1部分成交关闭,2完全成交关闭，3未成交关闭)',
    use_status			INT(10)			 NOT NULL DEFAULT 0 COMMENT '(0未使用,1部分使用,2完全使用)',
    UNIQUE (order_id),
    INDEX (seg_id),
    INDEX (status,use_status)
)
ENGINE = InnoDB
DEFAULT CHARSET = utf8;


CREATE TABLE IF NOT EXISTS buy_tx (
    id           		INT UNSIGNED     NOT NULL AUTO_INCREMENT PRIMARY KEY,
    seg_id      		VARCHAR(255)     NOT NULL,
    seg_price			decimal(20,8)	 NOT NULL,
    order_id      		VARCHAR(255)     NOT NULL,
    buy_number			decimal(20,8)	 NOT NULL,
    buy_price			decimal(20,8)	 NOT NULL,
    deal_number			decimal(20,8)	 NOT NULL DEFAULT 0,
    deal_price			decimal(20,8)	 NOT NULL DEFAULT 0,
    deal_fee			decimal(20,8)	 NOT NULL DEFAULT 0,
    deal_amount			decimal(20,8)	 NOT NULL DEFAULT 0,
    create_time			TIMESTAMP        NOT NULL DEFAULT CURRENT_TIMESTAMP,
    update_time			TIMESTAMP        NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    status				INT(10)			 NOT NULL DEFAULT 0 COMMENT '(0进行中,1部分成交关闭,2完全成交关闭，3未成交关闭)',
    UNIQUE (order_id)
)
ENGINE = InnoDB
DEFAULT CHARSET = utf8;

CREATE TABLE IF NOT EXISTS buy_history (
    id           		INT UNSIGNED     NOT NULL AUTO_INCREMENT PRIMARY KEY,
    buy_tx_id			INT UNSIGNED	 NOT NULL,
    seg_id      		VARCHAR(255)     NOT NULL,
    seg_price			decimal(20,8)	 NOT NULL,
    order_id      		VARCHAR(255)     NOT NULL,
    deal_number			decimal(20,8)	 NOT NULL,
    deal_price			decimal(20,8)	 NOT NULL,
    deal_fee			decimal(20,8)	 NOT NULL DEFAULT 0,
    deal_amount			decimal(20,8)	 NOT NULL DEFAULT 0,
    create_time			TIMESTAMP        NOT NULL DEFAULT CURRENT_TIMESTAMP,
    update_time			TIMESTAMP        NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    status				INT(10)			 NOT NULL DEFAULT 0 COMMENT '(0进行中,1部分成交关闭,2完全成交关闭，3未成交关闭)',
    UNIQUE (order_id)
)
ENGINE = InnoDB
DEFAULT CHARSET = utf8;


CREATE TABLE IF NOT EXISTS sale_history (
    id           		INT UNSIGNED     NOT NULL AUTO_INCREMENT PRIMARY KEY,
    sale_tx_id			INT UNSIGNED	 NOT NULL,
    seg_id      		VARCHAR(255)     NOT NULL,
    seg_price			decimal(20,8)	 NOT NULL,
    order_id      		VARCHAR(255)     NOT NULL,
    deal_number			decimal(20,8)	 NOT NULL DEFAULT 0,
    deal_price			decimal(20,8)	 NOT NULL DEFAULT 0,
    deal_fee			decimal(20,8)	 NOT NULL DEFAULT 0,
    deal_amount			decimal(20,8)	 NOT NULL DEFAULT 0,
    create_time			TIMESTAMP        NOT NULL DEFAULT CURRENT_TIMESTAMP,
    update_time			TIMESTAMP        NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    status				INT(10)			 NOT NULL DEFAULT 0 COMMENT '(0进行中,1部分成交关闭,2完全成交关闭，3未成交关闭)',
    UNIQUE (order_id)
)
ENGINE = InnoDB
DEFAULT CHARSET = utf8;