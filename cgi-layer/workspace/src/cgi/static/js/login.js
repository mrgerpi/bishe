var vm = new Vue({
    el: "#submitData",
    data: {
        email: "",
        password: "",
        checked: false
    },
    methods: {
        "login": function () {
            var baseurl = "http://120.79.247.113:8080";
            axios.post(baseurl + "/user", "email=" + this.email + "&password" + this.password)
            .then(function(response) {
				if (response.status == 200) {
					if (response.data != "ok") {
						alert("error: " + response.data);
					} else {
						alert("login succ");
					}
				} else {
					alert("please request again");
				}
            })
            .catch(function(error) {
                alert("error: " + error)
            })
        }
    },
})
