import random

class MatrixGen():

    def i(self,size):
        total_size = size*size
        start_num = 1
        File_object = open(r"inputs/i"+str(size)+".txt","w+")
        for start_num in range(size):
            next_num = start_num+1
            print("Line: "+ str(start_num))
            for y in range(size):
                if next_num > size:
                    next_num = 1
                if y != 0:
                    File_object.write(" ")
                File_object.write(str(next_num))
                next_num+=1
            File_object.write(' ')


    def r(self,size):
        total_size = size*size
        start_num = 1
        File_object = open(r"inputs/r"+str(size)+".txt","w+")
        for start_num in range(size):
            for y in range(size):
                next_num = random.randint(1,size)
                if y != 0:
                    File_object.write(" ")
                File_object.write(str(next_num))
                next_num+=1
            File_object.write(' ')

def main():
    size = int(input("Choose size, beware 10k+ will take a while: "))
    MatrixGen().i(size)
    #MatrixGen().r(size)
    print("Created two files i"+str(size)+".txt and r"+str(size)+".txt")

if __name__ == "__main__":
    main()

