
                    output += 's';
                    output += '\n';
                    stock->b1=stock_p;
                }
                else{
                    output += "No Trade\n";
                    if(stock->b < stock_p){
                        stock->b = std::max(stock->b, stock_p);
                    }
                }
            }
            else{
                if(stock_p <= stock->t){
                    output += stock_name;
                    output += ' ';
                    output += std::to_string(int(stock_p));
                    output += ' ';
                    output += 'b';