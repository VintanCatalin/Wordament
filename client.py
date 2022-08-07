import socket


def cautare_cuv(a, cuv):
    for i in a:
        if i == cuv:
            return 0
    return -1


def main():
    nrvec = 0
    score = 0
    arr = []
    cuv_scor = "ClientInetPY:"
    t = 0

    host = '192.168.100.9'
    port = 5000
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
    s.connect((host, port))

    k = s.recv(30).decode()

    for i in range(0, 5):
        for j in range(0, 5):
            print(k[nrvec], end=' ')
            nrvec = nrvec + 1
        print()

    while 1:
        cuv = input("Scrie aici:")
        print(cuv)
        s.sendall(cuv.encode())

        if cuv == "STOP":
            print("S-a oprit clientul")
            break

        if cuv == "NEXT":
            t = 0
            nrvec = 0
            arr.clear()
            cuvnou = s.recv(25).decode()

            for i in range(0, 5):
                for j in range(0, 5):
                    print(cuvnou[nrvec], end=' ')
                    nrvec = nrvec + 1
                print()
        else:
            rez2 = str(s.recv(1).decode())
            print(rez2)
            if rez2 == "0":
                print("Cuvantul exista in dictionar")
                rezc = str(s.recv(1).decode())
                print(rezc)
                if rezc == "0":
                    print("CORECT")
                    if cautare_cuv(arr, cuv) == -1:
                        score = score+(len(cuv))
                        t = t + 1
                        arr.append(cuv)
                    else:
                        print("Cuvantul a mai fost folosit odata")
                    print("Score: ", score)
                else:
                    print("INCORECT!!")
            else:
                print("Cuvantul nu este in dictionar")

    fi = open("scor_iclient.txt", "w+")
    fni = open("Scoruri_Jucatori_iclient.txt", "w+")
    cuv_scor1 = str(score)
    cuv_scor = cuv_scor + cuv_scor1
    print(cuv_scor)
    fi.write(cuv_scor)
    fi.seek(0,0)
    cuvdinfisier = fi.readline()
    print(cuvdinfisier)
    s.sendall(cuvdinfisier.encode())
    while 1:
        cuvdinserver = s.recv(50).decode()
        if cuvdinserver == "":
            break
        print(cuvdinserver)
        fni.write(cuvdinserver)
    fi.close()
    fni.close()
    s.close()


main()
