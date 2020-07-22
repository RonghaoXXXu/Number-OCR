import numpy as np
import PIL.Image as Image


def img_vector(imgFile):
    img=Image.open(imgFile).convert('L')
    img_arr=np.array(img,'i')
    img_normalize=np.round(img_arr/255)
    img_arr2=np.reshape(img,img_normalize,(1,-1))

    return img_arr2

def read_convert(imgFilelist):
    datalabel=[]
    dataNum=len(imgFilelist)
    dataMat=np.zeros(dataNum,40000)
    for i in range(dataNum):
        imgNameStr=imgFilelist[i]
        imgName=get_img_name_str(imgNameStr)
        classtag=imgName.spilt('_')[0]+"."+imgName.spilt('_')[1]
        datalabel.append(classtag)
        dataMat[i,:] = img_vector(imgNameStr)

    return dataMat, datalabel

def get_img_name_str(str1):
    str2=str1.split('/')
    num=len(str2)
    str3=str2[num-1]

    return str3

def read_all():
    className=['1_1','1_2','1_3','2_1','2_2','2_3','3_1','3_2','3_3',
              '4_1','4_2','4_3','5_1','5_2','5_3','6_1','6_2','6_3']
    base_data_path='/home/feyker/Desktop/SVM/tem/'
    filelist=get_file_list(base_data_path)
    dataMat,datalabel=read_convert(filelist)

    for c in className:
        train_data_path_ = base_data_path+c+'/'
        flist_ = []
        for i in range(1,7):
            for j in range(1,4):
                for k in range(0,500):
                    p=train_data_path_+str(i)+'_'+str(j)+'_'+str(k) +'.jpg'
                    flist_.append(p)
        dataMat_, datalabel_ = read_convert(flist_)
        dataMat = np.concatenate((dataMat, dataMat_), axis=0)
        datalabel = np.concatenate((datalabel, datalabel_), axis=0)
    
    return dataMat, datalabel


def get_file_list(base_path):
    filelist=[]
    for i in range(1,7):
        for j in range(1,4):
            for k in range(0,500):
                path=base_path+str(i)+'_'+str(j)+'/'+str(i)+'_'+str(j)+'_'+str(k) +'.jpg'
                filelist.append(path)
    return filelist
            



        

