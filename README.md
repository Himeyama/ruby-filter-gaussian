# ruby-filter-gaussian

## 概要
ガウシアンフィルターの Ruby 拡張

## インストール

```sh
gem install specific_install
gem specific_install -l "git://github.com/himeyama/univ-research-processing.git"
```

or

**Gemfile**
```rb
gem "univ-research-processing", github: "himeyama/univ-research-processing.git", branch: :main
```

## 使用法
<dt><code>Gaussian.filter1d(ary, sd) -> Array</code></dt>
<dd>
    <p>データへガウシアンフィルター処理を行います。</p>
    <dl>
        <dt>[PARAM] ary:</dt>
        <dd>一次元のデータ</dd>
        <dt>[PARAM] sd:</dt>
        <dd>ガウシアンフィルターの標準偏差。</dd>
    </dl>

```rb
require "Gaussian"

Gaussian.filter1d [1, 2, 3, 4, 5], 1
#=> [1.4270409503911738, 2.0678220347792573, 3.0, 3.932177965220743, 4.572959049608826]
```   

</dd>