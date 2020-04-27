function [B] = FD(A,l)
  [n,d] = size(A); % n and d are assigned
  B = zeros(2*l,d); % Set B all zeros (2l × d) matrix  
  % Iterate through A's rows
  for i = 1:n
      % Checker to see if they swapped
      swap = false;
      % insert into zero row of B 
      for j = 1:2*l
          maxValue = max(B(j,:));
          minValue = min(B(j,:));
          if maxValue == 0 && minValue == 0
              B(j,:) = A(i,:);
              break;
          end
      end
      % Check if any zero rows
      for j = 1:2*l
          maxValue = max(B(j,:));
          minValue = min(B(j,:));
          if maxValue == 0 && minValue == 0
              swap = true;
              break;
          end
      end    
      % if (B has no zero-valued rows)
      if swap == false
          [U, S, V ] = svd(B); % [U, S, V ] = svd(B)
          delta = (S(l,l)^2); % Set δi = σl2
          S1 = zeros(2*l,d);
          for j = 1:l -1 % iterating through each vector
              S1(j,j) = sqrt((S(j,j)^2) - delta); % doing the calculation
          end
          B = S1 * V';          
      end         
  end
end