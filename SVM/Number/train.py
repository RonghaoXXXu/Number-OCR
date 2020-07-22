import train_pre as pre
from sklearn import svm  #svm包
from sklearn.externals import joblib #保存模型
from sklearn.decomposition import PCA  #降维
import time #计算训练时间


def main():
    train_data,train_label=pre.read_all()
    name=['1_1','2_1','3_1']
    path='/home/feyker/Desktop/SVM/tem/'
    filelist=[]
    for i in name:
        for k in range(0,500):
            path_=path+i+'/'+i+'_'+str(k) +'.jpg'
            filelist.append(path)
    test_data,test_label=pre.read_convert(filelist)

    time=time.time()
    #PCA降维
    pca = PCA(n_components=0.8, whiten=True)
    print('start pca...')
    train_x = pca.fit_transform(train_data)
    test_x = pca.transform(test_data)
    print(train_x.shape)

    svc=svm.SVC(kernel = 'rbf', C = 6)
    svc.fit(train_data,train_label)
    pred = svc.predict(test_data)


    joblib.dump(svc, 'model.m')

    score = svc.score(test_data, test_label)
    print(u'准确率：%f,花费时间：%.2fs' % (score, time.time() - time))