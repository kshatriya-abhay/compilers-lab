
import re

#file input 
file_name = input()
file = open(file_name,"r")

#each token of form (token,line_no)
tokens = []
line_no=0
for line in file:
    line_no = line_no + 1

    #tokenize a line and store in a list temp
    temp = re.findall(r"[\w']+|[^a-zA-Z0-9\s\t]", line)

    if(len(temp)!=0):
        #remove comments followed by // from the line
        if "/" in temp:
            start = temp.index("/")
            if start+1 < len(temp) and temp[start+1] == "/":
                for i in range(start,len(temp)):
                    temp[i] = "  "
        for idx,val in enumerate(temp):
            temp[idx] = [temp[idx],line_no]

        tokens = tokens + temp


#to remove all tokens of form /*...*/
index = 0
while index < len(tokens):
    if tokens[index][0] == "/" and tokens[index+1][0] == "*":
        while index+1<len(tokens) and not (tokens[index][0]=="*" and tokens[index+1][0]=="/"):
            tokens[index][0] = "  "
            index = index + 1
        if index+1<len(tokens) and tokens[index][0]=="*" and tokens[index+1][0]=="/":
            tokens[index][0]=""
            tokens[index+1][0]="  "
    index = index+1

tokens  = [x for x in tokens if not x[0].isspace()]
# print(tokens)


#list for names of classes
class_list = []

#dict to store line no at which the following occur
line_nos = {"class":[],"object":[],"constructor":[],"inherited":[],"operator":[]}


#comment flag
c_flag=int(0)

#string flag
s_flag=int(0)

#bracket flag
b_flag=int(0)

#class_flag
class_flag = int(0)

#curly bracket count
count = int(0)

for idx,val in enumerate(tokens):
    # print(count)

    if val[0] == "class" and c_flag == 0 and s_flag == 0:
        
        #class 

        if not tokens[idx+1][0].isalnum():
            #error no class name specified
            t=0
        class_flag = 1
        class_list.append(tokens[idx+1][0])

        if(tokens[idx+2][0]=="{"):

            line_nos["class"].append(val[1])

        elif(tokens[idx+2][0]==":"):

            line_nos["class"].append(val[1])
            line_nos["inherited"].append(val[1])

    elif val[0] in class_list and c_flag == 0 and s_flag == 0 and b_flag==0:

        #either constructor or class object

        if tokens[idx+1][0]=="(":
            #class constructor
            def_flag = int(0)
            temp = idx
            while temp < len(tokens) and tokens[temp][0] != ";":
                temp = temp + 1
                if temp < len(tokens) and tokens[temp][0] == "{":
                    def_flag = 1
                    break

            #constructor
            if idx-1>=0 and tokens[idx-1][0].isalnum():
                #constructor with return type error
                k=0

            elif def_flag==1 and idx-1 >=0 and tokens[idx-1][0]!="~":
                line_nos["constructor"].append(val[1])

        elif tokens[idx+1][0].isalnum() and tokens[idx+1][0]!="operator" and class_flag==0:

            #class object 
            curly = int(0)
            temp = idx
            while temp < len(tokens) and tokens[temp][0] !=";":
                temp = temp + 1
                if temp < len(tokens) and tokens[temp][0] == "{":
                    curly=1
                    break
            if curly == 0 :
                line_nos["object"].append(val[1])

        elif tokens[idx+1][0] == "*":
            new_flag = int(0)
            temp = idx
            while temp < len(tokens) and tokens[temp][0] !=";":
                temp = temp + 1
                if temp < len(tokens) and tokens[temp][0] == "new":
                    new_flag=1
                    break
            if new_flag == 1 :
                line_nos["object"].append(val[1])


    elif val[0] == "operator" and c_flag == 0 and s_flag == 0 :
        #operator overloading
        def_flag = int(0)
        temp = idx
        while temp < len(tokens) and tokens[temp][0]!=";":
            temp = temp + 1
            if temp < len(tokens) and tokens[temp][0]=="{":
                def_flag = 1
                break

        if def_flag == 1:
            line_nos["operator"].append(val[1])


    #check for comments and cout statements
    elif idx+1 < len(tokens) and val[0] == "/" and tokens[idx+1][0]=="*" :
        c_flag = 1

    elif val[0] == '"' and s_flag==0:
        s_flag = 1

    elif val[0] == '"' and s_flag==1:
        s_flag = 0

    elif idx+1 < len(tokens) and val[0]=="*" and tokens[idx+1][0]=="/":
        c_flag = 0

    elif val[0] == "(":
        b_flag=1

    elif val[0] == ")":
        b_flag=0

    elif val[0] == "{":
        count = count + 1

    elif val[0] == "}":
        count = count - 1
        if count == 0 and class_flag == 1:
            class_flag = 0

#print(line_nos)
print("1) Object Declarations: ",len(set(line_nos["object"])))
print("2) Class Definitions: ",len(set(line_nos["class"])))
print("3) Constructor Definitions: ",len(set(line_nos["constructor"])))
print("4) Inherited Class Definitions: ",len(set(line_nos["inherited"])))
print("5) Operator Overloaded Function Definitions: ",len(set(line_nos["operator"])))
# print(class_list)
