function [D1y] = FirstDer(x,y)

    D1y = zeros(1, size(y,2));
    
    for k = 1:length(x)
        
        if k == 1
            x0 = x(k);
            x1 = x(k + 1);
            x2 = x(k + 2);
            y0 = y(k);
            y1 = y(k + 1);
            y2 = y(k + 2);
                        
        elseif k == length(x)
            x0 = x(k - 2);
            x1 = x(k - 1);
            x2 = x(k);
            y0 = y(k - 2);
            y1 = y(k - 1);
            y2 = y(k);
                      
        else
            x0 = x(k - 1);
            x1 = x(k);
            x2 = x(k + 1);
            y0 = y(k - 1);
            y1 = y(k);
            y2 = y(k + 1);            
                      
        end
        
        temp = @(xx) (y0 * ((2 * xx) - x1 - x2)) / ((x0 - x1) * (x0 - x2)) ...
                + (y1 * ((2 * xx) - x0 - x2)) / ((x1 - x0) * (x1 - x2)) ...
                + (y2 * ((2 * xx) - x0 - x1)) / ((x2 - x0) * (x2 - x1)) ...
                + (1/6) * (x0 - x1) * (x0 - x2);
        D1y(k) = temp(x(k));
        
    end
    
    return;
end   