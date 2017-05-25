appA.controller('HomeController', function ($scope,mySocket) {
  mySocket.on('temp', function (data) {
    //console.log(data.message);
    if(data.message!= 'nan'){
      $scope.bar = data.message;
    }
  });
});
