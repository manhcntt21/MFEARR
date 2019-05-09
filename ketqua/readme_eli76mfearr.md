Tham so:

number_generation = 500 (so luong the he)
emp = 0.3 (tham so de lai ghep)
dimension_one = 76 (so luong dinh cua task1)
dimension_two = 56 (so luong dinh cua task2) = dimension_one - 20
epsilon = 0.01 

t = number_generation/10 (so the he de tinh ASRD)
ASRD (Accumulated Survival Rate of Divergents) 

Giai thich ket qua trong file: Moi muc duoc gan cach boi ------------------------------------------

+	Dau tien là solution task1
+	Soluion task2
+	Ma tran distance
+ 	chi phi cua task 1 (da tinh vong lai tu thanh pho cuoi cung toi thanh pho dau tien)
+	chi phi cua task 2 (da tinh vong lai tu thanh pho cuoi cung toi thanh pho dau tien)
+ 	rank cua task 1 (da tinh vong lai tu thanh pho cuoi cung toi thanh pho dau tien)
+	rank cua task 2 (da tinh vong lai tu thanh pho cuoi cung toi thanh pho dau tien)
+ 	skill factor
+ 	scalar_fitness
+	divergent (tinh so luong ca the bi danh dau la divergent trong moi the he)
+	lamda: tinh ti le divergent[i+1]/divergent[i]
	Quy uoc:	divergents[0] = 0
			lamda[0] = 0
			neu divergent[i] va divergent[i+1] deu bang 0, thi lamda[i] = 0
			neu divergent[i] < divergent[i+i] thi lamda[i] = 1
			neu divergent[i] va divergent[i+1] bang nhau va khac 0  thi lamda[i] = 1
			neu divergent[i] > divergent[i+1] thi lamda[i] = divergent[i+1] / divergent[i]
			

			Trong file, phan lon cac gia tri sau cua lamda[i] = 0, la vi khi thiet lap rmp = 0, thi luc dau khong can phai tinh lamda nua.
		
