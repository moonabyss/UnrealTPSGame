@echo off
e:
cd e:\jenkins
java -jar agent.jar -jnlpUrl http://localhost:8080/manage/computer/home1/jenkins-agent.jnlp -secret d51a96ed8010c8a615e1683b4cd772f9117b21906504e255b25746296e253b49 -workDir "e:\Jenkins"