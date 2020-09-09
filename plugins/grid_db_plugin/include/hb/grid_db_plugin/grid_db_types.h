//
// Created by 李卫东 on 2019-02-26.
//

#pragma once

#include <string>
#include <vector>

namespace hb{ namespace plugin {
        using namespace std;
        enum db_tx_order_status{
            db_tx_processing = 0,
            db_tx_portion_closed = 1,
            db_tx_complet_closed = 2,
            db_tx_untrad_closed = 3
        };
        enum db_tx_use_status {
            db_tx_unused = 0,
            db_tx_portion_used = 1,
            db_tx_complet_used = 2
        };
        struct sale_tx_type
        {
            uint32_t id;              		    //INT UNSIGNED     NOT NULL AUTO_INCREMENT PRIMARY KEY,
            string seg_id;       		        //VARCHAR(255)     NOT NULL,
            double seg_price;                   //decimal(20,4)	   NOT NULL DEFAULT 0
            string order_id;       			    //VARCHAR(255)     NOT NULL,
            double sale_number;			        //decimal(20,4)	   NOT NULL,
            double sale_price;			        //decimal(20,4)	   NOT NULL,
            double deal_number;			        //decimal(20,4)	   NOT NULL DEFAULT 0,
            double deal_price;			        //decimal(20,4)	   NOT NULL DEFAULT 0,
            double deal_fee;			        //decimal(20,4)	   NOT NULL DEFAULT 0,
            double history_fee;			        //decimal(20,4)	   NOT NULL DEFAULT 0,
            double deal_amount;				    //decimal(20,4)	   NOT NULL DEFAULT 0 COMMENT 'used_buy_amount+free_buy_amount=deal_amount',
            double used_buy_amount;				//decimal(20,4)	   NOT NULL DEFAULT 0,	
            double free_buy_amount;				//decimal(20,4)	   NOT NULL DEFAULT 0 ,
            double valid_sale_number;			//decimal(20,4)	   NOT NULL DEFAULT 0 ,
            uint64_t create_time;				//TIMESTAMP        NOT NULL DEFAULT CURRENT_TIMESTAMP,
            uint64_t update_time;				//TIMESTAMP        NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
            db_tx_order_status status;			//INT(10)		   NOT NULL DEFAULT 0 COMMENT '(0进行中,1部分成交关闭,2完全成交关闭，3未成交关闭)',
            db_tx_use_status use_status;		//INT(10)		   NOT NULL DEFAULT 0 COMMENT '(0未使用,1部分使用,2完全使用)	',
        };
        typedef vector<sale_tx_type> sale_tx_array_type;
        struct buy_tx_type
        {
            uint32_t id;              		    //INT UNSIGNED      NOT NULL AUTO_INCREMENT PRIMARY KEY,
            string seg_id;       		        //VARCHAR(255)      NOT NULL,
            double seg_price;                   //decimal(20,4)	   NOT NULL DEFAULT 0
            string order_id;       			    //VARCHAR(255)      NOT NULL,
            double buy_number;			        //decimal(20,4)	    NOT NULL,
            double buy_price;			        //decimal(20,4)	    NOT NULL,
            double history_fee;			        //decimal(20,4)	   NOT NULL DEFAULT 0,
            double deal_fee;			        //decimal(20,4)	   NOT NULL DEFAULT 0,
            double deal_number;			        //decimal(20,4)	    NOT NULL DEFAULT 0,
            double deal_price;			        //decimal(20,4)	    NOT NULL DEFAULT 0,
            double deal_amount;				    //decimal(20,4)	    NOT NULL DEFAULT 0 COMMENT 'used_buy_amount+free_buy_amount=deal_amount',
            uint64_t create_time;				//TIMESTAMP         NOT NULL DEFAULT CURRENT_TIMESTAMP,
            uint64_t update_time;				//TIMESTAMP         NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
            db_tx_order_status status;			//INT(10)			NOT NULL DEFAULT 0 COMMENT '(0进行中,1部分成交关闭,2完全成交关闭，3未成交关闭)',
        };
        typedef vector<buy_tx_type> buy_tx_array_type;
}}